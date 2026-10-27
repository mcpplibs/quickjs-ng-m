// The QuickJS-ng embedding API through the module: runtimes and contexts,
// evaluation, C functions (the adapted JS_NewCFunction), exceptions, JSON,
// promises and the job queue, ES modules through a loader, classes with
// opaque data and a finalizer, property tables built with JS_CFUNC_DEF in a
// constant-initialised array — spelled as quickjs.h documents them.
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.quickjs_ng;

using namespace mcpplibs::mwrap::quickjs_ng;          // the static inline API, adapted
using namespace mcpplibs::mwrap::quickjs_ng::macros;  // JS_UNDEFINED, JS_EVAL_TYPE_GLOBAL …

namespace {

struct Engine {
    JSRuntime* rt = JS_NewRuntime();
    JSContext* ctx = JS_NewContext(rt);
    ~Engine() {
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
    }
    Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    JSValue eval(std::string_view code, int flags = JS_EVAL_TYPE_GLOBAL, const char* file = "<test>") {
        const std::string text(code);
        return JS_Eval(ctx, text.c_str(), text.size(), file, flags);
    }
    std::string string(JSValue v) {
        const char* s = JS_ToCString(ctx, v);
        std::string out = s ? s : "<null>";
        JS_FreeCString(ctx, s);
        return out;
    }
    std::string exception() {
        JSValue e = JS_GetException(ctx);
        std::string out = string(e);
        JS_FreeValue(ctx, e);
        return out;
    }
};

JSValue add(JSContext* ctx, JSValueConst, int argc, JSValueConst* argv) {
    int64_t a = 0, b = 0;
    if (argc < 2 || JS_ToInt64(ctx, &a, argv[0]) || JS_ToInt64(ctx, &b, argv[1])) return JS_ThrowTypeError(ctx, "add(a, b) takes two numbers");
    return JS_NewInt64(ctx, a + b);
}

JSValue twice(JSContext* ctx, JSValueConst, int, JSValueConst* argv) {
    int32_t n = 0;
    JS_ToInt32(ctx, &n, argv[0]);
    return JS_NewInt32(ctx, n * 2);
}

constexpr JSCFunctionListEntry math_table[] = {
    JS_CFUNC_DEF("twice", 1, twice),
    JS_PROP_INT32_DEF("answer", 42, JS_PROP_CONFIGURABLE),
    JS_PROP_STRING_DEF("name", "maths", JS_PROP_CONFIGURABLE),
};

JSClassID point_class = 0;
int finalized = 0;
struct Point {
    double x, y;
};
void point_finalizer(JSRuntime*, JSValueConst v) {
    delete static_cast<Point*>(JS_GetOpaque(v, point_class));
    ++finalized;
}

}  // namespace

TEST(Api, EvaluateAndConvert) {
    Engine e;
    JSValue v = e.eval("[1, 2, 3].map(x => x * 2).join('-')");
    ASSERT_TRUE(JS_IsString(v));
    EXPECT_EQ(e.string(v), "2-4-6");
    JS_FreeValue(e.ctx, v);
    v = e.eval("6 * 7");
    int32_t n = 0;
    EXPECT_EQ(JS_ToInt32(e.ctx, &n, v), 0);
    EXPECT_EQ(n, 42);
    EXPECT_TRUE(JS_IsNumber(v));
    EXPECT_EQ(JS_VALUE_GET_TAG(v), JS_TAG_INT);
    EXPECT_TRUE(JS_IsUndefined(JS_UNDEFINED) && JS_IsNull(JS_NULL) && JS_IsBool(JS_TRUE));
}

TEST(Api, CFunctionsAndExceptions) {
    Engine e;
    JSValue global = JS_GetGlobalObject(e.ctx);
    JS_SetPropertyStr(e.ctx, global, "add", JS_NewCFunction(e.ctx, add, "add", 2));
    JSValue v = e.eval("add(40, 2)");
    EXPECT_EQ(e.string(v), "42");
    JS_FreeValue(e.ctx, v);
    v = e.eval("add('x')");
    ASSERT_TRUE(JS_IsException(v));
    EXPECT_EQ(e.exception(), "TypeError: add(a, b) takes two numbers");
    v = e.eval("throw new RangeError('out of range')");
    ASSERT_TRUE(JS_IsException(v));
    EXPECT_EQ(e.exception(), "RangeError: out of range");
    v = e.eval("let = ;");
    ASSERT_TRUE(JS_IsException(v));
    EXPECT_NE(e.exception().find("SyntaxError"), std::string::npos);
    JS_FreeValue(e.ctx, global);
}

TEST(Api, PropertyTablesFromConstantInitialisers) {
    Engine e;
    JSValue global = JS_GetGlobalObject(e.ctx);
    JSValue maths = JS_NewObject(e.ctx);
    ASSERT_EQ(JS_SetPropertyFunctionList(e.ctx, maths, math_table, 3), 0);
    JS_SetPropertyStr(e.ctx, global, "maths", maths);
    JSValue v = e.eval("`${maths.name}: ${maths.twice(maths.answer)}`");
    EXPECT_EQ(e.string(v), "maths: 84");
    JS_FreeValue(e.ctx, v);
    JS_FreeValue(e.ctx, global);
}

TEST(Api, JsonPromisesAndJobs) {
    Engine e;
    const std::string_view json = R"({"a": [1, 2], "b": "c"})";
    JSValue parsed = JS_ParseJSON(e.ctx, json.data(), json.size(), "<json>");
    ASSERT_FALSE(JS_IsException(parsed));
    JSValue text = JS_JSONStringify(e.ctx, parsed, JS_UNDEFINED, JS_UNDEFINED);
    EXPECT_EQ(e.string(text), R"({"a":[1,2],"b":"c"})");
    JS_FreeValue(e.ctx, text);
    JS_FreeValue(e.ctx, parsed);

    JSValue p = e.eval("Promise.resolve(20).then(x => x + 1).then(x => x * 2)");
    ASSERT_TRUE(JS_IsPromise(p));
    EXPECT_EQ(JS_PromiseState(e.ctx, p), JS_PROMISE_PENDING);
    JSContext* job_ctx = nullptr;
    while (JS_IsJobPending(e.rt)) ASSERT_GE(JS_ExecutePendingJob(e.rt, &job_ctx), 0);
    EXPECT_EQ(JS_PromiseState(e.ctx, p), JS_PROMISE_FULFILLED);
    JSValue r = JS_PromiseResult(e.ctx, p);
    EXPECT_EQ(e.string(r), "42");
    JS_FreeValue(e.ctx, r);
    JS_FreeValue(e.ctx, p);
}

TEST(Api, ModulesThroughALoader) {
    Engine e;
    JS_SetModuleLoaderFunc(
        e.rt, nullptr,
        [](JSContext* ctx, const char* name, void*) -> JSModuleDef* {
            if (std::string_view(name) != "maths") return nullptr;
            constexpr std::string_view source = "export const square = x => x * x;";
            JSValue m = JS_Eval(ctx, source.data(), source.size(), name, JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
            if (JS_IsException(m)) return nullptr;
            auto* def = static_cast<JSModuleDef*>(JS_VALUE_GET_PTR(m));
            JS_FreeValue(ctx, m);
            return def;
        },
        nullptr);
    JSValue result = e.eval("import { square } from 'maths'; globalThis.out = square(12);", JS_EVAL_TYPE_MODULE, "<main>");
    ASSERT_FALSE(JS_IsException(result)) << e.exception();
    JSContext* job_ctx = nullptr;
    while (JS_IsJobPending(e.rt)) JS_ExecutePendingJob(e.rt, &job_ctx);
    JS_FreeValue(e.ctx, result);
    JSValue out = e.eval("out");
    EXPECT_EQ(e.string(out), "144");
    JS_FreeValue(e.ctx, out);
}

TEST(Api, ClassesWithOpaqueDataAndFinalizers) {
    finalized = 0;
    {
        Engine e;
        JS_NewClassID(e.rt, &point_class);
        JSClassDef def{};
        def.class_name = "Point";
        def.finalizer = point_finalizer;
        ASSERT_EQ(JS_NewClass(e.rt, point_class, &def), 0);
        JSValue obj = JS_NewObjectClass(e.ctx, static_cast<int>(point_class));
        JS_SetOpaque(obj, new Point{3, 4});
        auto* p = static_cast<Point*>(JS_GetOpaque2(e.ctx, obj, point_class));
        ASSERT_NE(p, nullptr);
        EXPECT_DOUBLE_EQ(std::hypot(p->x, p->y), 5.0);
        JS_FreeValue(e.ctx, obj);
    }
    EXPECT_EQ(finalized, 1);
}

TEST(Api, Version) {
    EXPECT_EQ(std::string_view(JS_GetVersion()), "0.17.0");
    EXPECT_EQ(QJS_VERSION_MAJOR, 0);
    EXPECT_EQ(QJS_VERSION_MINOR, 17);
}
