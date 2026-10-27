// The raw header and the import in one translation unit (the side header
// first, the canonical order): the module's names are the very same
// entities; the macros a function cannot be work beside them; the header's
// own values equal the mapped constants. While a header's macro is defined
// it expands even inside a qualified name, so `macros::JS_PROP_C_W_E` is only
// reachable with the macro set aside.
#include <mwrap/quickjs_ng/macros.hpp>
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.quickjs_ng;

namespace {
constexpr int raw_cwe = JS_PROP_C_W_E, raw_module = JS_EVAL_TYPE_MODULE;
JSValue header_undefined() { return JS_UNDEFINED; }  // the header's, a call in C++
JSValue header_exception() { return JS_EXCEPTION; }
}  // namespace

#pragma push_macro("JS_PROP_C_W_E")
#pragma push_macro("JS_EVAL_TYPE_MODULE")
#undef JS_PROP_C_W_E
#undef JS_EVAL_TYPE_MODULE
static_assert(mcpplibs::mwrap::quickjs_ng::macros::JS_PROP_C_W_E == raw_cwe);
static_assert(mcpplibs::mwrap::quickjs_ng::macros::JS_EVAL_TYPE_MODULE == raw_module);
#pragma pop_macro("JS_EVAL_TYPE_MODULE")
#pragma pop_macro("JS_PROP_C_W_E")

TEST(HeaderMix, SameEntitiesAndValues) {
    static_assert(std::is_same_v<decltype(&::JS_NewRuntime), JSRuntime* (*)()>);
    static_assert(std::is_same_v<JSCFunctionListEntry, ::JSCFunctionListEntry>);
    namespace m = mcpplibs::mwrap::quickjs_ng::macros;
    const JSValue u = header_undefined(), x = header_exception();
    EXPECT_EQ(u.tag, m::JS_UNDEFINED.tag);
    EXPECT_EQ(u.u.int32, m::JS_UNDEFINED.u.int32);
    EXPECT_EQ(x.tag, m::JS_EXCEPTION.tag);
    // The adapted static inline functions agree with the header's own.
    JSRuntime* rt = JS_NewRuntime();
    JSContext* ctx = JS_NewContext(rt);
    JSValue a = mcpplibs::mwrap::quickjs_ng::JS_NewInt32(ctx, 7), b = ::JS_NewInt32(ctx, 7);
    EXPECT_EQ(a.tag, b.tag);
    EXPECT_EQ(a.u.int32, b.u.int32);
    JSValue s = ::JS_NewString(ctx, "x"), t = ::JS_NewString(ctx, "y");
    JS_FreeValues(ctx, s, t);  // the variadic macro
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
}
