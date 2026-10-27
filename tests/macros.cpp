import mcpplibs.mwrap.quickjs_ng.macros;
namespace macros = mcpplibs::mwrap::quickjs_ng::macros;
static_assert(!macros::JS_NAN_BOXING);
static_assert(macros::JS_VALUE_GET_TAG(macros::JS_UNDEFINED) == macros::JS_TAG_UNDEFINED);
static_assert(macros::JS_VALUE_GET_TAG(macros::JS_EXCEPTION) == macros::JS_TAG_EXCEPTION);
static_assert(macros::JS_VALUE_GET_BOOL(macros::JS_TRUE) == 1 && macros::JS_VALUE_GET_BOOL(macros::JS_FALSE) == 0);
static_assert(macros::JS_VALUE_GET_INT(macros::JS_MKVAL(macros::JS_TAG_INT, -7)) == -7);
static_assert(macros::JS_VALUE_IS_BOTH_INT(macros::JS_MKVAL(macros::JS_TAG_INT, 1), macros::JS_MKVAL(macros::JS_TAG_INT, 2)));
static_assert(macros::JS_VALUE_IS_NAN(macros::JS_NAN) && !macros::JS_VALUE_IS_NAN(macros::JS_NULL));
static_assert(!macros::JS_VALUE_HAS_REF_COUNT(macros::JS_NULL));
static_assert(macros::JS_PROP_C_W_E == (macros::JS_PROP_CONFIGURABLE | macros::JS_PROP_WRITABLE | macros::JS_PROP_ENUMERABLE));
static_assert(macros::JS_EVAL_TYPE_MODULE == (1 << 0) && macros::JS_EVAL_FLAG_COMPILE_ONLY == (1 << 5));
static_assert(macros::JS_ATOM_NULL == 0 && macros::JS_INVALID_CLASS_ID == 0);
constexpr auto entry = macros::JS_PROP_INT32_DEF("answer", 42, macros::JS_PROP_C_W_E);
static_assert(entry.def_type == macros::JS_DEF_PROP_INT32 && entry.u.i32 == 42 && entry.prop_flags == macros::JS_PROP_C_W_E);
int main() { return macros::QJS_VERSION_SUFFIX[0] == '\0' && macros::JS_DEFAULT_STACK_SIZE == 1024 * 1024 ? 0 : 1; }
