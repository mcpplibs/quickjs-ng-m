// qjs — runs a JavaScript file through import mcpplibs.mwrap.quickjs_ng: a
// script, or an ES module (JS_DetectModule, unless told) whose imports are
// files, resolved relative to the importer, loaded by their import attributes
// (`with { type: "json" | "text" | "bytes" }`, a .json file as JSON) as
// quickjs-libc's loader does. The globals are print() and gc(), as upstream's
// test runner adds them; quickjs-libc itself (qjs:std, qjs:os) is not part of
// the engine package. Pending jobs run until none is left. An exception —
// and, with --track-rejections, a promise rejection nobody handled — is
// printed and the exit status is 1.
//
//   qjs [--module | --script] [--track-rejections] FILE.js
import std;
import mcpplibs.mwrap.quickjs_ng;

using namespace mcpplibs::mwrap::quickjs_ng;
using namespace mcpplibs::mwrap::quickjs_ng::macros;

namespace {

std::optional<std::string> read(const std::filesystem::path& file) {
    std::ifstream in(file, std::ios::binary);
    if (!in) return std::nullopt;
    return std::string((std::istreambuf_iterator<char>(in)), {});
}

void report(JSContext* ctx, JSValueConst error) {
    const char* text = JS_ToCString(ctx, error);
    std::println(std::cerr, "{}", text ? text : "(an error that does not convert to a string)");
    JS_FreeCString(ctx, text);
    if (JS_IsObject(error)) {
        JSValue stack = JS_GetPropertyStr(ctx, error, "stack");
        if (!JS_IsUndefined(stack)) {
            const char* s = JS_ToCString(ctx, stack);
            if (s && *s) std::print(std::cerr, "{}", s);
            JS_FreeCString(ctx, s);
        }
        JS_FreeValue(ctx, stack);
    }
}

JSValue print(JSContext* ctx, JSValueConst, int argc, JSValueConst* argv) {
    std::string line;
    for (int i = 0; i < argc; ++i) {
        const char* s = JS_ToCString(ctx, argv[i]);
        if (!s && JS_IsObject(argv[i])) {  // no string conversion: [object …], as upstream's print
            JS_FreeValue(ctx, JS_GetException(ctx));
            JSValue t = JS_ToObjectString(ctx, argv[i]);
            s = JS_ToCString(ctx, t);
            JS_FreeValue(ctx, t);
        }
        if (!s) return JS_EXCEPTION;
        line += (i ? " " : "") + std::string(s);
        JS_FreeCString(ctx, s);
    }
    std::println("{}", line);
    return JS_UNDEFINED;
}

JSValue gc(JSContext* ctx, JSValueConst, int, JSValueConst*) {
    JS_RunGC(JS_GetRuntime(ctx));
    return JS_UNDEFINED;
}

// The module's value, the "default" export of a JSON, text or bytes module.
int default_export(JSContext* ctx, JSModuleDef* m) { return JS_SetModuleExport(ctx, m, "default", JS_GetModulePrivateValue(ctx, m)); }

JSModuleDef* load(JSContext* ctx, const char* name, void*, JSValueConst attributes) {
    std::string type = "js";
    if (!JS_IsUndefined(attributes)) {
        JSValue t = JS_GetPropertyStr(ctx, attributes, "type");
        if (JS_IsException(t)) return nullptr;
        if (JS_IsString(t)) {
            const char* s = JS_ToCString(ctx, t);
            type = s ? s : "";
            JS_FreeCString(ctx, s);
        }
        JS_FreeValue(ctx, t);
        if (type != "js" && type != "json" && type != "text" && type != "bytes") {
            JS_ThrowTypeError(ctx, "unsupported module type: '%s'", type.c_str());
            return nullptr;
        }
    }
    if (type == "js" && std::string_view(name).ends_with(".json")) type = "json";
    auto source = read(name);
    if (!source) {
        JS_ThrowReferenceError(ctx, "could not load module filename '%s'", name);
        return nullptr;
    }
    if (type == "js") {
        JSValue m = JS_Eval(ctx, source->c_str(), source->size(), name, JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
        if (JS_IsException(m)) return nullptr;
        auto* def = static_cast<JSModuleDef*>(JS_VALUE_GET_PTR(m));
        JS_FreeValue(ctx, m);
        return def;
    }
    JSValue value = JS_UNDEFINED;
    if (type == "json") {
        value = JS_ParseJSON(ctx, source->c_str(), source->size(), name);
    } else if (type == "text") {
        value = JS_NewStringLen(ctx, source->c_str(), source->size());
    } else {
        value = JS_NewUint8ArrayCopy(ctx, reinterpret_cast<const std::uint8_t*>(source->data()), source->size());
        if (!JS_IsException(value)) {
            JSValue buffer = JS_GetTypedArrayBuffer(ctx, value, nullptr, nullptr, nullptr);
            JS_SetImmutableArrayBuffer(buffer, true);
            JS_FreeValue(ctx, buffer);
        }
    }
    if (JS_IsException(value)) return nullptr;
    JSModuleDef* m = JS_NewCModule(ctx, name, default_export);
    if (!m) {
        JS_FreeValue(ctx, value);
        return nullptr;
    }
    JS_AddModuleExport(ctx, m, "default");
    JS_SetModulePrivateValue(ctx, m, value);
    return m;
}

// Rejected promises nobody has handled yet, by identity.
struct Rejections {
    std::vector<std::pair<JSValue, JSValue>> pending;  // promise, reason
};

void track(JSContext* ctx, JSValueConst promise, JSValueConst reason, bool handled, void* opaque) {
    auto& r = static_cast<Rejections*>(opaque)->pending;
    auto it = std::ranges::find_if(r, [&](const auto& p) { return JS_IsStrictEqual(ctx, p.first, promise); });
    if (handled) {
        if (it != r.end()) {
            JS_FreeValue(ctx, it->first);
            JS_FreeValue(ctx, it->second);
            r.erase(it);
        }
    } else if (it == r.end()) {
        r.emplace_back(JS_DupValue(ctx, promise), JS_DupValue(ctx, reason));
    }
}

}  // namespace

int main(int argc, char** argv) {
    std::optional<bool> module_flag;
    bool track_rejections = false;
    const char* file = nullptr;
    for (int i = 1; i < argc; ++i) {
        const std::string_view a = argv[i];
        if (a == "--module") module_flag = true;
        else if (a == "--script") module_flag = false;
        else if (a == "--track-rejections") track_rejections = true;
        else if (!file && !a.starts_with("--")) file = argv[i];
        else {
            std::println(std::cerr, "usage: qjs [--module | --script] [--track-rejections] FILE.js");
            return 2;
        }
    }
    if (!file) {
        std::println(std::cerr, "usage: qjs [--module | --script] [--track-rejections] FILE.js");
        return 2;
    }
    auto source = read(file);
    if (!source) {
        std::println(std::cerr, "qjs: cannot read {}", file);
        return 1;
    }
    JSRuntime* rt = JS_NewRuntime();
    JSContext* ctx = JS_NewContext(rt);
    Rejections rejections;
    JS_SetModuleLoaderFunc2(rt, nullptr, load, nullptr, nullptr);
    if (track_rejections) JS_SetHostPromiseRejectionTracker(rt, track, &rejections);
    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global, "print", JS_NewCFunction(ctx, print, "print", 1));
    JS_SetPropertyStr(ctx, global, "gc", JS_NewCFunction(ctx, gc, "gc", 0));
    JS_FreeValue(ctx, global);

    const bool module = module_flag.value_or(JS_DetectModule(source->c_str(), source->size()));
    JSValue result = JS_Eval(ctx, source->c_str(), source->size(), file, module ? JS_EVAL_TYPE_MODULE : JS_EVAL_TYPE_GLOBAL);
    bool ok = !JS_IsException(result);
    if (!ok) {
        JSValue e = JS_GetException(ctx);
        report(ctx, e);
        JS_FreeValue(ctx, e);
    }
    // Every pending job; a job's exception is reported and fails the run.
    for (JSContext* job_ctx = nullptr;;) {
        const int r = JS_ExecutePendingJob(rt, &job_ctx);
        if (r == 0) break;
        if (r < 0) {
            JSValue e = JS_GetException(job_ctx);
            report(job_ctx, e);
            JS_FreeValue(job_ctx, e);
            ok = false;
        }
    }
    // A module's evaluation is a promise: rejected, it failed.
    if (ok && module && JS_IsPromise(result) && JS_PromiseState(ctx, result) == JS_PROMISE_REJECTED) {
        JSValue e = JS_PromiseResult(ctx, result);
        report(ctx, e);
        JS_FreeValue(ctx, e);
        ok = false;
    }
    for (auto& [promise, reason] : rejections.pending) {
        std::print(std::cerr, "unhandled promise rejection: ");
        report(ctx, reason);
        JS_FreeValue(ctx, promise);
        JS_FreeValue(ctx, reason);
        ok = false;
    }
    JS_FreeValue(ctx, result);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return ok ? 0 : 1;
}
