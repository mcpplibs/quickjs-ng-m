// mcpplibs.mwrap.quickjs_ng.macros — the macros of <quickjs.h> (a named module
// cannot export a macro), under their original names in
// mcpplibs::mwrap::quickjs_ng::macros:
//
//   - the flag and limit constants (JS_PROP_*, JS_EVAL_*, JS_GPN_*, JS_DUMP_*,
//     JS_WRITE_OBJ_*, JS_READ_OBJ_*, JS_DEF_*, QJS_VERSION_* …) as
//     `inline constexpr`, each of its macro's type, and the JS_TAG_*
//     enumerators of quickjs.h's unnamed enum (TU-local, so not exportable
//     as they are);
//   - the value representation as constexpr: JS_MKVAL, JS_MKPTR, the
//     JS_VALUE_GET_* accessors, JS_NULL / JS_UNDEFINED / JS_TRUE / JS_FALSE /
//     JS_EXCEPTION / JS_UNINITIALIZED / JS_NAN as constants (the header's are
//     calls in C++), JS_VALUE_IS_NAN, JSValueConst;
//   - the JSCFunctionListEntry initialisers (JS_CFUNC_DEF, JS_CGETSET_DEF,
//     JS_PROP_*_DEF, JS_OBJECT_DEF, JS_ALIAS_DEF …) as constexpr functions,
//     usable in a constant-initialised table as the macros are.
//
// JS_CFUNC_SPECIAL_DEF pastes tokens into an enumerator and a member name;
// JS_FreeValues / JS_FreeAtoms (and their RT forms) free each value before
// the next argument is evaluated; QUICKJS_NG is a test for #if: those stay
// macros, in <mwrap/quickjs_ng/macros.hpp>, included before the import.
//
// The struct representation of JSValue, every 64-bit target's, is the one
// supported; the NaN-boxed one of 32-bit targets is refused at compile time.
// tools/check_exports.py accounts for every macro.
module;
#include <quickjs.h>

#if defined(JS_NAN_BOXING) && JS_NAN_BOXING
#error "mcpplibs.mwrap.quickjs_ng supports the struct representation of JSValue (64-bit targets); this target NaN-boxes it"
#endif

namespace mwrap_quickjs_ng_capture {
inline constexpr auto value_JS_ABORT_ON_LEAKS = JS_ABORT_ON_LEAKS;
inline constexpr auto value_JS_ATOM_NULL = JS_ATOM_NULL;
inline constexpr auto value_JS_CALL_FLAG_CONSTRUCTOR = JS_CALL_FLAG_CONSTRUCTOR;
inline constexpr auto value_JS_DEFAULT_STACK_SIZE = JS_DEFAULT_STACK_SIZE;
inline constexpr auto value_JS_DEF_ALIAS = JS_DEF_ALIAS;
inline constexpr auto value_JS_DEF_CFUNC = JS_DEF_CFUNC;
inline constexpr auto value_JS_DEF_CGETSET = JS_DEF_CGETSET;
inline constexpr auto value_JS_DEF_CGETSET_MAGIC = JS_DEF_CGETSET_MAGIC;
inline constexpr auto value_JS_DEF_OBJECT = JS_DEF_OBJECT;
inline constexpr auto value_JS_DEF_PROP_BOOL = JS_DEF_PROP_BOOL;
inline constexpr auto value_JS_DEF_PROP_DOUBLE = JS_DEF_PROP_DOUBLE;
inline constexpr auto value_JS_DEF_PROP_INT32 = JS_DEF_PROP_INT32;
inline constexpr auto value_JS_DEF_PROP_INT64 = JS_DEF_PROP_INT64;
inline constexpr auto value_JS_DEF_PROP_STRING = JS_DEF_PROP_STRING;
inline constexpr auto value_JS_DEF_PROP_SYMBOL = JS_DEF_PROP_SYMBOL;
inline constexpr auto value_JS_DEF_PROP_UNDEFINED = JS_DEF_PROP_UNDEFINED;
inline constexpr auto value_JS_DUMP_ATOMS = JS_DUMP_ATOMS;
inline constexpr auto value_JS_DUMP_ATOM_LEAKS = JS_DUMP_ATOM_LEAKS;
inline constexpr auto value_JS_DUMP_BYTECODE_FINAL = JS_DUMP_BYTECODE_FINAL;
inline constexpr auto value_JS_DUMP_BYTECODE_HEX = JS_DUMP_BYTECODE_HEX;
inline constexpr auto value_JS_DUMP_BYTECODE_PASS1 = JS_DUMP_BYTECODE_PASS1;
inline constexpr auto value_JS_DUMP_BYTECODE_PASS2 = JS_DUMP_BYTECODE_PASS2;
inline constexpr auto value_JS_DUMP_BYTECODE_PC2LINE = JS_DUMP_BYTECODE_PC2LINE;
inline constexpr auto value_JS_DUMP_BYTECODE_STACK = JS_DUMP_BYTECODE_STACK;
inline constexpr auto value_JS_DUMP_BYTECODE_STEP = JS_DUMP_BYTECODE_STEP;
inline constexpr auto value_JS_DUMP_FREE = JS_DUMP_FREE;
inline constexpr auto value_JS_DUMP_GC = JS_DUMP_GC;
inline constexpr auto value_JS_DUMP_GC_FREE = JS_DUMP_GC_FREE;
inline constexpr auto value_JS_DUMP_LEAKS = JS_DUMP_LEAKS;
inline constexpr auto value_JS_DUMP_MEM = JS_DUMP_MEM;
inline constexpr auto value_JS_DUMP_MODULE_RESOLVE = JS_DUMP_MODULE_RESOLVE;
inline constexpr auto value_JS_DUMP_OBJECTS = JS_DUMP_OBJECTS;
inline constexpr auto value_JS_DUMP_PROMISE = JS_DUMP_PROMISE;
inline constexpr auto value_JS_DUMP_READ_OBJECT = JS_DUMP_READ_OBJECT;
inline constexpr auto value_JS_DUMP_SHAPES = JS_DUMP_SHAPES;
inline constexpr auto value_JS_EVAL_FLAG_ASYNC = JS_EVAL_FLAG_ASYNC;
inline constexpr auto value_JS_EVAL_FLAG_BACKTRACE_BARRIER = JS_EVAL_FLAG_BACKTRACE_BARRIER;
inline constexpr auto value_JS_EVAL_FLAG_COMPILE_ONLY = JS_EVAL_FLAG_COMPILE_ONLY;
inline constexpr auto value_JS_EVAL_FLAG_STRICT = JS_EVAL_FLAG_STRICT;
inline constexpr auto value_JS_EVAL_FLAG_UNUSED = JS_EVAL_FLAG_UNUSED;
inline constexpr auto value_JS_EVAL_OPTIONS_VERSION = JS_EVAL_OPTIONS_VERSION;
inline constexpr auto value_JS_EVAL_TYPE_DIRECT = JS_EVAL_TYPE_DIRECT;
inline constexpr auto value_JS_EVAL_TYPE_GLOBAL = JS_EVAL_TYPE_GLOBAL;
inline constexpr auto value_JS_EVAL_TYPE_INDIRECT = JS_EVAL_TYPE_INDIRECT;
inline constexpr auto value_JS_EVAL_TYPE_MASK = JS_EVAL_TYPE_MASK;
inline constexpr auto value_JS_EVAL_TYPE_MODULE = JS_EVAL_TYPE_MODULE;
inline constexpr auto value_JS_GPN_ENUM_ONLY = JS_GPN_ENUM_ONLY;
inline constexpr auto value_JS_GPN_PRIVATE_MASK = JS_GPN_PRIVATE_MASK;
inline constexpr auto value_JS_GPN_SET_ENUM = JS_GPN_SET_ENUM;
inline constexpr auto value_JS_GPN_STRING_MASK = JS_GPN_STRING_MASK;
inline constexpr auto value_JS_GPN_SYMBOL_MASK = JS_GPN_SYMBOL_MASK;
inline constexpr auto value_JS_INVALID_CLASS_ID = JS_INVALID_CLASS_ID;
inline constexpr auto value_JS_PROP_AUTOINIT = JS_PROP_AUTOINIT;
inline constexpr auto value_JS_PROP_CONFIGURABLE = JS_PROP_CONFIGURABLE;
inline constexpr auto value_JS_PROP_C_W_E = JS_PROP_C_W_E;
inline constexpr auto value_JS_PROP_DEFINE_PROPERTY = JS_PROP_DEFINE_PROPERTY;
inline constexpr auto value_JS_PROP_ENUMERABLE = JS_PROP_ENUMERABLE;
inline constexpr auto value_JS_PROP_GETSET = JS_PROP_GETSET;
inline constexpr auto value_JS_PROP_HAS_CONFIGURABLE = JS_PROP_HAS_CONFIGURABLE;
inline constexpr auto value_JS_PROP_HAS_ENUMERABLE = JS_PROP_HAS_ENUMERABLE;
inline constexpr auto value_JS_PROP_HAS_GET = JS_PROP_HAS_GET;
inline constexpr auto value_JS_PROP_HAS_SET = JS_PROP_HAS_SET;
inline constexpr auto value_JS_PROP_HAS_SHIFT = JS_PROP_HAS_SHIFT;
inline constexpr auto value_JS_PROP_HAS_VALUE = JS_PROP_HAS_VALUE;
inline constexpr auto value_JS_PROP_HAS_WRITABLE = JS_PROP_HAS_WRITABLE;
inline constexpr auto value_JS_PROP_LENGTH = JS_PROP_LENGTH;
inline constexpr auto value_JS_PROP_NORMAL = JS_PROP_NORMAL;
inline constexpr auto value_JS_PROP_NO_ADD = JS_PROP_NO_ADD;
inline constexpr auto value_JS_PROP_NO_EXOTIC = JS_PROP_NO_EXOTIC;
inline constexpr auto value_JS_PROP_REFLECT_DEFINE_PROPERTY = JS_PROP_REFLECT_DEFINE_PROPERTY;
inline constexpr auto value_JS_PROP_THROW = JS_PROP_THROW;
inline constexpr auto value_JS_PROP_THROW_STRICT = JS_PROP_THROW_STRICT;
inline constexpr auto value_JS_PROP_TMASK = JS_PROP_TMASK;
inline constexpr auto value_JS_PROP_VARREF = JS_PROP_VARREF;
inline constexpr auto value_JS_PROP_WRITABLE = JS_PROP_WRITABLE;
inline constexpr auto value_JS_READ_OBJ_BYTECODE = JS_READ_OBJ_BYTECODE;
inline constexpr auto value_JS_READ_OBJ_REFERENCE = JS_READ_OBJ_REFERENCE;
inline constexpr auto value_JS_READ_OBJ_ROM_DATA = JS_READ_OBJ_ROM_DATA;
inline constexpr auto value_JS_READ_OBJ_SAB = JS_READ_OBJ_SAB;
inline constexpr auto value_JS_WRITE_OBJ_BSWAP = JS_WRITE_OBJ_BSWAP;
inline constexpr auto value_JS_WRITE_OBJ_BYTECODE = JS_WRITE_OBJ_BYTECODE;
inline constexpr auto value_JS_WRITE_OBJ_REFERENCE = JS_WRITE_OBJ_REFERENCE;
inline constexpr auto value_JS_WRITE_OBJ_SAB = JS_WRITE_OBJ_SAB;
inline constexpr auto value_JS_WRITE_OBJ_STRIP_DEBUG = JS_WRITE_OBJ_STRIP_DEBUG;
inline constexpr auto value_JS_WRITE_OBJ_STRIP_SOURCE = JS_WRITE_OBJ_STRIP_SOURCE;
inline constexpr auto value_QJS_VERSION_MAJOR = QJS_VERSION_MAJOR;
inline constexpr auto value_QJS_VERSION_MINOR = QJS_VERSION_MINOR;
inline constexpr auto value_QJS_VERSION_PATCH = QJS_VERSION_PATCH;
inline constexpr char value_QJS_VERSION_SUFFIX[] = QJS_VERSION_SUFFIX;
inline constexpr int value_JS_TAG_FIRST = JS_TAG_FIRST;
inline constexpr int value_JS_TAG_BIG_INT = JS_TAG_BIG_INT;
inline constexpr int value_JS_TAG_SYMBOL = JS_TAG_SYMBOL;
inline constexpr int value_JS_TAG_STRING = JS_TAG_STRING;
inline constexpr int value_JS_TAG_STRING_ROPE = JS_TAG_STRING_ROPE;
inline constexpr int value_JS_TAG_MODULE = JS_TAG_MODULE;
inline constexpr int value_JS_TAG_FUNCTION_BYTECODE = JS_TAG_FUNCTION_BYTECODE;
inline constexpr int value_JS_TAG_OBJECT = JS_TAG_OBJECT;
inline constexpr int value_JS_TAG_INT = JS_TAG_INT;
inline constexpr int value_JS_TAG_BOOL = JS_TAG_BOOL;
inline constexpr int value_JS_TAG_NULL = JS_TAG_NULL;
inline constexpr int value_JS_TAG_UNDEFINED = JS_TAG_UNDEFINED;
inline constexpr int value_JS_TAG_UNINITIALIZED = JS_TAG_UNINITIALIZED;
inline constexpr int value_JS_TAG_CATCH_OFFSET = JS_TAG_CATCH_OFFSET;
inline constexpr int value_JS_TAG_EXCEPTION = JS_TAG_EXCEPTION;
inline constexpr int value_JS_TAG_SHORT_BIG_INT = JS_TAG_SHORT_BIG_INT;
inline constexpr int value_JS_TAG_FLOAT64 = JS_TAG_FLOAT64;
}  // namespace mwrap_quickjs_ng_capture

// Every mapped name, so the definitions below are not rewritten.
#undef JS_ABORT_ON_LEAKS
#undef JS_ATOM_NULL
#undef JS_CALL_FLAG_CONSTRUCTOR
#undef JS_DEFAULT_STACK_SIZE
#undef JS_DEF_ALIAS
#undef JS_DEF_CFUNC
#undef JS_DEF_CGETSET
#undef JS_DEF_CGETSET_MAGIC
#undef JS_DEF_OBJECT
#undef JS_DEF_PROP_BOOL
#undef JS_DEF_PROP_DOUBLE
#undef JS_DEF_PROP_INT32
#undef JS_DEF_PROP_INT64
#undef JS_DEF_PROP_STRING
#undef JS_DEF_PROP_SYMBOL
#undef JS_DEF_PROP_UNDEFINED
#undef JS_DUMP_ATOMS
#undef JS_DUMP_ATOM_LEAKS
#undef JS_DUMP_BYTECODE_FINAL
#undef JS_DUMP_BYTECODE_HEX
#undef JS_DUMP_BYTECODE_PASS1
#undef JS_DUMP_BYTECODE_PASS2
#undef JS_DUMP_BYTECODE_PC2LINE
#undef JS_DUMP_BYTECODE_STACK
#undef JS_DUMP_BYTECODE_STEP
#undef JS_DUMP_FREE
#undef JS_DUMP_GC
#undef JS_DUMP_GC_FREE
#undef JS_DUMP_LEAKS
#undef JS_DUMP_MEM
#undef JS_DUMP_MODULE_RESOLVE
#undef JS_DUMP_OBJECTS
#undef JS_DUMP_PROMISE
#undef JS_DUMP_READ_OBJECT
#undef JS_DUMP_SHAPES
#undef JS_EVAL_FLAG_ASYNC
#undef JS_EVAL_FLAG_BACKTRACE_BARRIER
#undef JS_EVAL_FLAG_COMPILE_ONLY
#undef JS_EVAL_FLAG_STRICT
#undef JS_EVAL_FLAG_UNUSED
#undef JS_EVAL_OPTIONS_VERSION
#undef JS_EVAL_TYPE_DIRECT
#undef JS_EVAL_TYPE_GLOBAL
#undef JS_EVAL_TYPE_INDIRECT
#undef JS_EVAL_TYPE_MASK
#undef JS_EVAL_TYPE_MODULE
#undef JS_GPN_ENUM_ONLY
#undef JS_GPN_PRIVATE_MASK
#undef JS_GPN_SET_ENUM
#undef JS_GPN_STRING_MASK
#undef JS_GPN_SYMBOL_MASK
#undef JS_INVALID_CLASS_ID
#undef JS_PROP_AUTOINIT
#undef JS_PROP_CONFIGURABLE
#undef JS_PROP_C_W_E
#undef JS_PROP_DEFINE_PROPERTY
#undef JS_PROP_ENUMERABLE
#undef JS_PROP_GETSET
#undef JS_PROP_HAS_CONFIGURABLE
#undef JS_PROP_HAS_ENUMERABLE
#undef JS_PROP_HAS_GET
#undef JS_PROP_HAS_SET
#undef JS_PROP_HAS_SHIFT
#undef JS_PROP_HAS_VALUE
#undef JS_PROP_HAS_WRITABLE
#undef JS_PROP_LENGTH
#undef JS_PROP_NORMAL
#undef JS_PROP_NO_ADD
#undef JS_PROP_NO_EXOTIC
#undef JS_PROP_REFLECT_DEFINE_PROPERTY
#undef JS_PROP_THROW
#undef JS_PROP_THROW_STRICT
#undef JS_PROP_TMASK
#undef JS_PROP_VARREF
#undef JS_PROP_WRITABLE
#undef JS_READ_OBJ_BYTECODE
#undef JS_READ_OBJ_REFERENCE
#undef JS_READ_OBJ_ROM_DATA
#undef JS_READ_OBJ_SAB
#undef JS_WRITE_OBJ_BSWAP
#undef JS_WRITE_OBJ_BYTECODE
#undef JS_WRITE_OBJ_REFERENCE
#undef JS_WRITE_OBJ_SAB
#undef JS_WRITE_OBJ_STRIP_DEBUG
#undef JS_WRITE_OBJ_STRIP_SOURCE
#undef QJS_VERSION_MAJOR
#undef QJS_VERSION_MINOR
#undef QJS_VERSION_PATCH
#undef QJS_VERSION_SUFFIX
#undef JS_MKVAL
#undef JS_MKPTR
#undef JS_VALUE_GET_TAG
#undef JS_VALUE_GET_NORM_TAG
#undef JS_VALUE_GET_INT
#undef JS_VALUE_GET_BOOL
#undef JS_VALUE_GET_FLOAT64
#undef JS_VALUE_GET_SHORT_BIG_INT
#undef JS_VALUE_GET_PTR
#undef JS_TAG_IS_FLOAT64
#undef JS_NAN
#undef JS_VALUE_IS_BOTH_INT
#undef JS_VALUE_IS_BOTH_FLOAT
#undef JS_VALUE_HAS_REF_COUNT
#undef JS_NULL
#undef JS_UNDEFINED
#undef JS_FALSE
#undef JS_TRUE
#undef JS_EXCEPTION
#undef JS_UNINITIALIZED
#undef JSValueConst
#undef JS_NAN_BOXING
#undef JS_CFUNC_DEF
#undef JS_CFUNC_DEF2
#undef JS_CFUNC_MAGIC_DEF
#undef JS_ITERATOR_NEXT_DEF
#undef JS_CGETSET_DEF
#undef JS_CGETSET_DEF2
#undef JS_CGETSET_MAGIC_DEF
#undef JS_CGETSET_MAGIC_DEF2
#undef JS_PROP_STRING_DEF
#undef JS_PROP_INT32_DEF
#undef JS_PROP_INT64_DEF
#undef JS_PROP_DOUBLE_DEF
#undef JS_PROP_U2D_DEF
#undef JS_PROP_UNDEFINED_DEF
#undef JS_PROP_SYMBOL_DEF
#undef JS_PROP_BOOL_DEF
#undef JS_OBJECT_DEF
#undef JS_ALIAS_DEF
#undef JS_ALIAS_BASE_DEF

export module mcpplibs.mwrap.quickjs_ng.macros;

import std;

namespace c = mwrap_quickjs_ng_capture;

export namespace mcpplibs::mwrap::quickjs_ng::macros {

// ── constants ────────────────────────────────────────────────────────────
inline constexpr auto JS_ABORT_ON_LEAKS = c::value_JS_ABORT_ON_LEAKS;
inline constexpr auto JS_ATOM_NULL = c::value_JS_ATOM_NULL;
inline constexpr auto JS_CALL_FLAG_CONSTRUCTOR = c::value_JS_CALL_FLAG_CONSTRUCTOR;
inline constexpr auto JS_DEFAULT_STACK_SIZE = c::value_JS_DEFAULT_STACK_SIZE;
inline constexpr auto JS_DEF_ALIAS = c::value_JS_DEF_ALIAS;
inline constexpr auto JS_DEF_CFUNC = c::value_JS_DEF_CFUNC;
inline constexpr auto JS_DEF_CGETSET = c::value_JS_DEF_CGETSET;
inline constexpr auto JS_DEF_CGETSET_MAGIC = c::value_JS_DEF_CGETSET_MAGIC;
inline constexpr auto JS_DEF_OBJECT = c::value_JS_DEF_OBJECT;
inline constexpr auto JS_DEF_PROP_BOOL = c::value_JS_DEF_PROP_BOOL;
inline constexpr auto JS_DEF_PROP_DOUBLE = c::value_JS_DEF_PROP_DOUBLE;
inline constexpr auto JS_DEF_PROP_INT32 = c::value_JS_DEF_PROP_INT32;
inline constexpr auto JS_DEF_PROP_INT64 = c::value_JS_DEF_PROP_INT64;
inline constexpr auto JS_DEF_PROP_STRING = c::value_JS_DEF_PROP_STRING;
inline constexpr auto JS_DEF_PROP_SYMBOL = c::value_JS_DEF_PROP_SYMBOL;
inline constexpr auto JS_DEF_PROP_UNDEFINED = c::value_JS_DEF_PROP_UNDEFINED;
inline constexpr auto JS_DUMP_ATOMS = c::value_JS_DUMP_ATOMS;
inline constexpr auto JS_DUMP_ATOM_LEAKS = c::value_JS_DUMP_ATOM_LEAKS;
inline constexpr auto JS_DUMP_BYTECODE_FINAL = c::value_JS_DUMP_BYTECODE_FINAL;
inline constexpr auto JS_DUMP_BYTECODE_HEX = c::value_JS_DUMP_BYTECODE_HEX;
inline constexpr auto JS_DUMP_BYTECODE_PASS1 = c::value_JS_DUMP_BYTECODE_PASS1;
inline constexpr auto JS_DUMP_BYTECODE_PASS2 = c::value_JS_DUMP_BYTECODE_PASS2;
inline constexpr auto JS_DUMP_BYTECODE_PC2LINE = c::value_JS_DUMP_BYTECODE_PC2LINE;
inline constexpr auto JS_DUMP_BYTECODE_STACK = c::value_JS_DUMP_BYTECODE_STACK;
inline constexpr auto JS_DUMP_BYTECODE_STEP = c::value_JS_DUMP_BYTECODE_STEP;
inline constexpr auto JS_DUMP_FREE = c::value_JS_DUMP_FREE;
inline constexpr auto JS_DUMP_GC = c::value_JS_DUMP_GC;
inline constexpr auto JS_DUMP_GC_FREE = c::value_JS_DUMP_GC_FREE;
inline constexpr auto JS_DUMP_LEAKS = c::value_JS_DUMP_LEAKS;
inline constexpr auto JS_DUMP_MEM = c::value_JS_DUMP_MEM;
inline constexpr auto JS_DUMP_MODULE_RESOLVE = c::value_JS_DUMP_MODULE_RESOLVE;
inline constexpr auto JS_DUMP_OBJECTS = c::value_JS_DUMP_OBJECTS;
inline constexpr auto JS_DUMP_PROMISE = c::value_JS_DUMP_PROMISE;
inline constexpr auto JS_DUMP_READ_OBJECT = c::value_JS_DUMP_READ_OBJECT;
inline constexpr auto JS_DUMP_SHAPES = c::value_JS_DUMP_SHAPES;
inline constexpr auto JS_EVAL_FLAG_ASYNC = c::value_JS_EVAL_FLAG_ASYNC;
inline constexpr auto JS_EVAL_FLAG_BACKTRACE_BARRIER = c::value_JS_EVAL_FLAG_BACKTRACE_BARRIER;
inline constexpr auto JS_EVAL_FLAG_COMPILE_ONLY = c::value_JS_EVAL_FLAG_COMPILE_ONLY;
inline constexpr auto JS_EVAL_FLAG_STRICT = c::value_JS_EVAL_FLAG_STRICT;
inline constexpr auto JS_EVAL_FLAG_UNUSED = c::value_JS_EVAL_FLAG_UNUSED;
inline constexpr auto JS_EVAL_OPTIONS_VERSION = c::value_JS_EVAL_OPTIONS_VERSION;
inline constexpr auto JS_EVAL_TYPE_DIRECT = c::value_JS_EVAL_TYPE_DIRECT;
inline constexpr auto JS_EVAL_TYPE_GLOBAL = c::value_JS_EVAL_TYPE_GLOBAL;
inline constexpr auto JS_EVAL_TYPE_INDIRECT = c::value_JS_EVAL_TYPE_INDIRECT;
inline constexpr auto JS_EVAL_TYPE_MASK = c::value_JS_EVAL_TYPE_MASK;
inline constexpr auto JS_EVAL_TYPE_MODULE = c::value_JS_EVAL_TYPE_MODULE;
inline constexpr auto JS_GPN_ENUM_ONLY = c::value_JS_GPN_ENUM_ONLY;
inline constexpr auto JS_GPN_PRIVATE_MASK = c::value_JS_GPN_PRIVATE_MASK;
inline constexpr auto JS_GPN_SET_ENUM = c::value_JS_GPN_SET_ENUM;
inline constexpr auto JS_GPN_STRING_MASK = c::value_JS_GPN_STRING_MASK;
inline constexpr auto JS_GPN_SYMBOL_MASK = c::value_JS_GPN_SYMBOL_MASK;
inline constexpr auto JS_INVALID_CLASS_ID = c::value_JS_INVALID_CLASS_ID;
inline constexpr auto JS_PROP_AUTOINIT = c::value_JS_PROP_AUTOINIT;
inline constexpr auto JS_PROP_CONFIGURABLE = c::value_JS_PROP_CONFIGURABLE;
inline constexpr auto JS_PROP_C_W_E = c::value_JS_PROP_C_W_E;
inline constexpr auto JS_PROP_DEFINE_PROPERTY = c::value_JS_PROP_DEFINE_PROPERTY;
inline constexpr auto JS_PROP_ENUMERABLE = c::value_JS_PROP_ENUMERABLE;
inline constexpr auto JS_PROP_GETSET = c::value_JS_PROP_GETSET;
inline constexpr auto JS_PROP_HAS_CONFIGURABLE = c::value_JS_PROP_HAS_CONFIGURABLE;
inline constexpr auto JS_PROP_HAS_ENUMERABLE = c::value_JS_PROP_HAS_ENUMERABLE;
inline constexpr auto JS_PROP_HAS_GET = c::value_JS_PROP_HAS_GET;
inline constexpr auto JS_PROP_HAS_SET = c::value_JS_PROP_HAS_SET;
inline constexpr auto JS_PROP_HAS_SHIFT = c::value_JS_PROP_HAS_SHIFT;
inline constexpr auto JS_PROP_HAS_VALUE = c::value_JS_PROP_HAS_VALUE;
inline constexpr auto JS_PROP_HAS_WRITABLE = c::value_JS_PROP_HAS_WRITABLE;
inline constexpr auto JS_PROP_LENGTH = c::value_JS_PROP_LENGTH;
inline constexpr auto JS_PROP_NORMAL = c::value_JS_PROP_NORMAL;
inline constexpr auto JS_PROP_NO_ADD = c::value_JS_PROP_NO_ADD;
inline constexpr auto JS_PROP_NO_EXOTIC = c::value_JS_PROP_NO_EXOTIC;
inline constexpr auto JS_PROP_REFLECT_DEFINE_PROPERTY = c::value_JS_PROP_REFLECT_DEFINE_PROPERTY;
inline constexpr auto JS_PROP_THROW = c::value_JS_PROP_THROW;
inline constexpr auto JS_PROP_THROW_STRICT = c::value_JS_PROP_THROW_STRICT;
inline constexpr auto JS_PROP_TMASK = c::value_JS_PROP_TMASK;
inline constexpr auto JS_PROP_VARREF = c::value_JS_PROP_VARREF;
inline constexpr auto JS_PROP_WRITABLE = c::value_JS_PROP_WRITABLE;
inline constexpr auto JS_READ_OBJ_BYTECODE = c::value_JS_READ_OBJ_BYTECODE;
inline constexpr auto JS_READ_OBJ_REFERENCE = c::value_JS_READ_OBJ_REFERENCE;
inline constexpr auto JS_READ_OBJ_ROM_DATA = c::value_JS_READ_OBJ_ROM_DATA;
inline constexpr auto JS_READ_OBJ_SAB = c::value_JS_READ_OBJ_SAB;
inline constexpr auto JS_WRITE_OBJ_BSWAP = c::value_JS_WRITE_OBJ_BSWAP;
inline constexpr auto JS_WRITE_OBJ_BYTECODE = c::value_JS_WRITE_OBJ_BYTECODE;
inline constexpr auto JS_WRITE_OBJ_REFERENCE = c::value_JS_WRITE_OBJ_REFERENCE;
inline constexpr auto JS_WRITE_OBJ_SAB = c::value_JS_WRITE_OBJ_SAB;
inline constexpr auto JS_WRITE_OBJ_STRIP_DEBUG = c::value_JS_WRITE_OBJ_STRIP_DEBUG;
inline constexpr auto JS_WRITE_OBJ_STRIP_SOURCE = c::value_JS_WRITE_OBJ_STRIP_SOURCE;
inline constexpr auto QJS_VERSION_MAJOR = c::value_QJS_VERSION_MAJOR;
inline constexpr auto QJS_VERSION_MINOR = c::value_QJS_VERSION_MINOR;
inline constexpr auto QJS_VERSION_PATCH = c::value_QJS_VERSION_PATCH;
inline constexpr auto& QJS_VERSION_SUFFIX = c::value_QJS_VERSION_SUFFIX;

// ── the JS_TAG_* enumerators ─────────────────────────────────────────────
inline constexpr int JS_TAG_FIRST = c::value_JS_TAG_FIRST;
inline constexpr int JS_TAG_BIG_INT = c::value_JS_TAG_BIG_INT;
inline constexpr int JS_TAG_SYMBOL = c::value_JS_TAG_SYMBOL;
inline constexpr int JS_TAG_STRING = c::value_JS_TAG_STRING;
inline constexpr int JS_TAG_STRING_ROPE = c::value_JS_TAG_STRING_ROPE;
inline constexpr int JS_TAG_MODULE = c::value_JS_TAG_MODULE;
inline constexpr int JS_TAG_FUNCTION_BYTECODE = c::value_JS_TAG_FUNCTION_BYTECODE;
inline constexpr int JS_TAG_OBJECT = c::value_JS_TAG_OBJECT;
inline constexpr int JS_TAG_INT = c::value_JS_TAG_INT;
inline constexpr int JS_TAG_BOOL = c::value_JS_TAG_BOOL;
inline constexpr int JS_TAG_NULL = c::value_JS_TAG_NULL;
inline constexpr int JS_TAG_UNDEFINED = c::value_JS_TAG_UNDEFINED;
inline constexpr int JS_TAG_UNINITIALIZED = c::value_JS_TAG_UNINITIALIZED;
inline constexpr int JS_TAG_CATCH_OFFSET = c::value_JS_TAG_CATCH_OFFSET;
inline constexpr int JS_TAG_EXCEPTION = c::value_JS_TAG_EXCEPTION;
inline constexpr int JS_TAG_SHORT_BIG_INT = c::value_JS_TAG_SHORT_BIG_INT;
inline constexpr int JS_TAG_FLOAT64 = c::value_JS_TAG_FLOAT64;

// ── the value representation (struct JSValue {{ JSValueUnion u; int64_t tag; }}) ──
inline constexpr bool JS_NAN_BOXING = false;
using JSValueConst = JSValue;

constexpr JSValue JS_MKVAL(int64_t tag, int32_t val) noexcept {
    JSValue v{};
    v.u.int32 = val;
    v.tag = tag;
    return v;
}
constexpr JSValue JS_MKPTR(int64_t tag, void* ptr) noexcept {
    JSValue v{};
    v.u.ptr = ptr;
    v.tag = tag;
    return v;
}
constexpr JSValue JS_MKNAN() noexcept {
    JSValue v{};
    v.u.float64 = std::numeric_limits<double>::quiet_NaN();
    v.tag = JS_TAG_FLOAT64;
    return v;
}
constexpr int32_t JS_VALUE_GET_TAG(JSValue v) noexcept { return static_cast<int32_t>(v.tag); }
constexpr int32_t JS_VALUE_GET_NORM_TAG(JSValue v) noexcept { return JS_VALUE_GET_TAG(v); }
constexpr int32_t JS_VALUE_GET_INT(JSValue v) noexcept { return v.u.int32; }
constexpr int32_t JS_VALUE_GET_BOOL(JSValue v) noexcept { return v.u.int32; }
constexpr double JS_VALUE_GET_FLOAT64(JSValue v) noexcept { return v.u.float64; }
constexpr int32_t JS_VALUE_GET_SHORT_BIG_INT(JSValue v) noexcept { return v.u.short_big_int; }
constexpr void* JS_VALUE_GET_PTR(JSValue v) noexcept { return v.u.ptr; }
constexpr bool JS_TAG_IS_FLOAT64(int32_t tag) noexcept { return static_cast<unsigned>(tag) == JS_TAG_FLOAT64; }
constexpr bool JS_VALUE_IS_BOTH_INT(JSValue v1, JSValue v2) noexcept { return (JS_VALUE_GET_TAG(v1) | JS_VALUE_GET_TAG(v2)) == 0; }
constexpr bool JS_VALUE_IS_BOTH_FLOAT(JSValue v1, JSValue v2) noexcept {
    return JS_TAG_IS_FLOAT64(JS_VALUE_GET_TAG(v1)) && JS_TAG_IS_FLOAT64(JS_VALUE_GET_TAG(v2));
}
constexpr bool JS_VALUE_HAS_REF_COUNT(JSValue v) noexcept {
    return static_cast<unsigned>(JS_VALUE_GET_TAG(v)) >= static_cast<unsigned>(JS_TAG_FIRST);
}
constexpr bool JS_VALUE_IS_NAN(JSValue v) noexcept {
    if (v.tag != JS_TAG_FLOAT64) return false;
    return (std::bit_cast<uint64_t>(v.u.float64) & 0x7fffffffffffffffu) > 0x7ff0000000000000u;
}
inline constexpr JSValue JS_NULL = JS_MKVAL(JS_TAG_NULL, 0);
inline constexpr JSValue JS_UNDEFINED = JS_MKVAL(JS_TAG_UNDEFINED, 0);
inline constexpr JSValue JS_FALSE = JS_MKVAL(JS_TAG_BOOL, 0);
inline constexpr JSValue JS_TRUE = JS_MKVAL(JS_TAG_BOOL, 1);
inline constexpr JSValue JS_EXCEPTION = JS_MKVAL(JS_TAG_EXCEPTION, 0);
inline constexpr JSValue JS_UNINITIALIZED = JS_MKVAL(JS_TAG_UNINITIALIZED, 0);
inline constexpr JSValue JS_NAN = JS_MKNAN();

// ── JSCFunctionListEntry initialisers ────────────────────────────────────
namespace detail {
constexpr JSCFunctionListEntry entry(const char* name, int prop_flags, int def_type, int magic) noexcept {
    JSCFunctionListEntry e{};
    e.name = name;
    e.prop_flags = static_cast<uint8_t>(prop_flags);
    e.def_type = static_cast<uint8_t>(def_type);
    e.magic = static_cast<int16_t>(magic);
    return e;
}
constexpr JSCFunctionListEntry function(const char* name, int prop_flags, int magic, int length, JSCFunctionEnum cproto,
                                        JSCFunctionType f) noexcept {
    JSCFunctionListEntry e = entry(name, prop_flags, JS_DEF_CFUNC, magic);
    e.u.func = {static_cast<uint8_t>(length), static_cast<uint8_t>(cproto), f};
    return e;
}
}  // namespace detail

constexpr JSCFunctionListEntry JS_CFUNC_DEF(const char* name, int length, JSCFunction* func1) noexcept {
    return detail::function(name, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE, 0, length, JS_CFUNC_generic, {.generic = func1});
}
constexpr JSCFunctionListEntry JS_CFUNC_DEF2(const char* name, int length, JSCFunction* func1, int prop_flags) noexcept {
    return detail::function(name, prop_flags, 0, length, JS_CFUNC_generic, {.generic = func1});
}
constexpr JSCFunctionListEntry JS_CFUNC_MAGIC_DEF(const char* name, int length, decltype(JSCFunctionType::generic_magic) func1,
                                                  int magic) noexcept {
    JSCFunctionType f{};
    f.generic_magic = func1;
    return detail::function(name, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE, magic, length, JS_CFUNC_generic_magic, f);
}
constexpr JSCFunctionListEntry JS_ITERATOR_NEXT_DEF(const char* name, int length, decltype(JSCFunctionType::iterator_next) func1,
                                                    int magic) noexcept {
    JSCFunctionType f{};
    f.iterator_next = func1;
    return detail::function(name, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE, magic, length, JS_CFUNC_iterator_next, f);
}
constexpr JSCFunctionListEntry JS_CGETSET_DEF2(const char* name, decltype(JSCFunctionType::getter) fgetter,
                                               decltype(JSCFunctionType::setter) fsetter, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_CGETSET, 0);
    JSCFunctionType get{}, set{};
    get.getter = fgetter;
    set.setter = fsetter;
    e.u.getset = {get, set};
    return e;
}
constexpr JSCFunctionListEntry JS_CGETSET_DEF(const char* name, decltype(JSCFunctionType::getter) fgetter,
                                              decltype(JSCFunctionType::setter) fsetter) noexcept {
    return JS_CGETSET_DEF2(name, fgetter, fsetter, JS_PROP_CONFIGURABLE);
}
constexpr JSCFunctionListEntry JS_CGETSET_MAGIC_DEF2(const char* name, decltype(JSCFunctionType::getter_magic) fgetter,
                                                     decltype(JSCFunctionType::setter_magic) fsetter, int magic,
                                                     int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_CGETSET_MAGIC, magic);
    JSCFunctionType get{}, set{};
    get.getter_magic = fgetter;
    set.setter_magic = fsetter;
    e.u.getset = {get, set};
    return e;
}
constexpr JSCFunctionListEntry JS_CGETSET_MAGIC_DEF(const char* name, decltype(JSCFunctionType::getter_magic) fgetter,
                                                    decltype(JSCFunctionType::setter_magic) fsetter, int magic) noexcept {
    return JS_CGETSET_MAGIC_DEF2(name, fgetter, fsetter, magic, JS_PROP_CONFIGURABLE);
}
constexpr JSCFunctionListEntry JS_PROP_STRING_DEF(const char* name, const char* cstr, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_PROP_STRING, 0);
    e.u.str = cstr;
    return e;
}
constexpr JSCFunctionListEntry JS_PROP_INT32_DEF(const char* name, int32_t val, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_PROP_INT32, 0);
    e.u.i32 = val;
    return e;
}
constexpr JSCFunctionListEntry JS_PROP_INT64_DEF(const char* name, int64_t val, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_PROP_INT64, 0);
    e.u.i64 = val;
    return e;
}
constexpr JSCFunctionListEntry JS_PROP_DOUBLE_DEF(const char* name, double val, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_PROP_DOUBLE, 0);
    e.u.f64 = val;
    return e;
}
constexpr JSCFunctionListEntry JS_PROP_U2D_DEF(const char* name, uint64_t val, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_PROP_DOUBLE, 0);
    e.u.u64 = val;
    return e;
}
constexpr JSCFunctionListEntry JS_PROP_UNDEFINED_DEF(const char* name, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_PROP_UNDEFINED, 0);
    e.u.i32 = 0;
    return e;
}
constexpr JSCFunctionListEntry JS_PROP_SYMBOL_DEF(const char* name, int32_t val, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_PROP_SYMBOL, 0);
    e.u.i32 = val;
    return e;
}
constexpr JSCFunctionListEntry JS_PROP_BOOL_DEF(const char* name, int32_t val, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_PROP_BOOL, 0);
    e.u.i32 = val;
    return e;
}
constexpr JSCFunctionListEntry JS_OBJECT_DEF(const char* name, const JSCFunctionListEntry* tab, int len, int prop_flags) noexcept {
    JSCFunctionListEntry e = detail::entry(name, prop_flags, JS_DEF_OBJECT, 0);
    e.u.prop_list = {tab, len};
    return e;
}
constexpr JSCFunctionListEntry JS_ALIAS_BASE_DEF(const char* name, const char* from, int base) noexcept {
    JSCFunctionListEntry e = detail::entry(name, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE, JS_DEF_ALIAS, 0);
    e.u.alias = {from, base};
    return e;
}
constexpr JSCFunctionListEntry JS_ALIAS_DEF(const char* name, const char* from) noexcept { return JS_ALIAS_BASE_DEF(name, from, -1); }

}  // namespace mcpplibs::mwrap::quickjs_ng::macros
