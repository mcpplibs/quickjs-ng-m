# Using mcpplibs.mwrap.quickjs_ng

`import mcpplibs.mwrap.quickjs_ng;` declares QuickJS-ng 0.17.0's embedding API as `<quickjs.h>` does. Types, enumerators and functions are the upstream globals, the very same entities as the header's, so a translation unit may include the header too. The engine is `compat.quickjs-ng`, built from the same release without quickjs-libc.

## Modules

| Module | What it holds |
|---|---|
| `mcpplibs.mwrap.quickjs_ng` | every type (`JSRuntime`, `JSContext`, `JSValue`, `JSClassDef`, `JSCFunctionListEntry` …), enumerator of a named enum (`JS_PROMISE_PENDING`, `JS_CFUNC_generic` …) and function (`JS_NewRuntime`, `JS_Eval`, `JS_FreeValue` … 264 of them); the `static inline` API, adapted in `mcpplibs::mwrap::quickjs_ng`; re-exports the one below |
| `mcpplibs.mwrap.quickjs_ng.macros` | the macros, in `mcpplibs::mwrap::quickjs_ng::macros` |

## The static inline API

27 functions of quickjs.h are `static inline`: `JS_NewInt32`, `JS_NewInt64`, `JS_NewBool`, `JS_NewFloat64`, `JS_NewString`, `JS_NewCFunction`, `JS_NewCFunctionMagic`, `JS_IsException`, `JS_IsUndefined`, `JS_IsString`, `JS_IsObject` and the other `JS_Is*`, `JS_ToCString`, `JS_ToCStringLen`, `JS_ToBoolean`, `JS_ToUint32`, `JS_AtomToCString` …

Internal linkage cannot be exported. Each is adapted as a function with the same name and signature in `mcpplibs::mwrap::quickjs_ng`. It is defined out of line by calling upstream's own inline body, so it gives the same result on every target at the cost of one call. `using namespace mcpplibs::mwrap::quickjs_ng;` makes code read as it does in C. A translation unit that also includes the header sees both: call the header's as `::JS_NewInt32`.

## Macros

A named module cannot export a macro. `mcpplibs::mwrap::quickjs_ng::macros` holds them under their original names:

- **Constants** as `inline constexpr`, each with its macro's type: `JS_PROP_*`, `JS_EVAL_TYPE_*`, `JS_EVAL_FLAG_*`, `JS_GPN_*`, `JS_DUMP_*`, `JS_WRITE_OBJ_*`, `JS_READ_OBJ_*`, `JS_DEF_*`, `JS_ATOM_NULL`, `JS_INVALID_CLASS_ID`, `JS_DEFAULT_STACK_SIZE`, `QJS_VERSION_*`.
- **`JS_TAG_*`**: the enumerators of quickjs.h's unnamed enum. A type with no name is TU-local, so they are constants here.
- **The value representation**, `constexpr`: `JS_MKVAL`, `JS_MKPTR`, `JS_VALUE_GET_TAG` and the other accessors, `JS_VALUE_IS_NAN`, `JS_VALUE_HAS_REF_COUNT`. `JS_NULL`, `JS_UNDEFINED`, `JS_TRUE`, `JS_FALSE`, `JS_EXCEPTION`, `JS_UNINITIALIZED` and `JS_NAN` are constants; the header's are calls in C++. `JSValueConst` is an alias of `JSValue`.
- **Property tables**: `JS_CFUNC_DEF`, `JS_CFUNC_MAGIC_DEF`, `JS_CGETSET_DEF`, `JS_PROP_INT32_DEF`, `JS_PROP_STRING_DEF`, `JS_OBJECT_DEF`, `JS_ALIAS_DEF` … are `constexpr` functions, so a `constexpr JSCFunctionListEntry table[] = {…}` is constant-initialised as the macros' is.

### What stays a macro

These stay macros in `<mwrap/quickjs_ng/macros.hpp>`, which is `<quickjs.h>`:

| Macro | Why |
|---|---|
| `JS_CFUNC_SPECIAL_DEF` | pastes its argument into an enumerator and a member name |
| `JS_FreeValues`, `JS_FreeValuesRT`, `JS_FreeAtoms`, `JS_FreeAtomsRT` | free each value before the next argument is evaluated |
| `QUICKJS_NG` | a test for `#if` |

Include it before the import. While a macro is defined it expands even inside a qualified name, so `macros::JS_PROP_C_W_E` is not reachable there; use the plain macro (see [tests/header_mix_test.cpp](../tests/header_mix_test.cpp)). [tools/check_exports.py](../tools/check_exports.py) accounts for all 157 macros.

## Targets

The struct representation of `JSValue`, the one every 64-bit target uses, is supported. The NaN-boxed representation of 32-bit targets is refused at compile time.

On Windows, a program that runs untrusted or deeply recursive JavaScript needs a thread stack larger than QuickJS's JS stack limit (`JS_DEFAULT_STACK_SIZE`, 1 MB). Otherwise the C stack overflows before the engine throws its `RangeError`. Upstream links its programs with an 8 MB stack; [examples/qjs](../examples/qjs/mcpp.toml) does the same (`-Wl,/STACK:8388608`). Alternatively, lower the limit with `JS_SetMaxStackSize`.

## Checks

- `mcpp test`: the embedding API through the module (evaluation, C functions, exceptions, JSON, promises and jobs, modules through a loader, classes with finalizers, property tables), the constants, and the header mixed with the import.
- `python3 tools/check_exports.py`: `src/quickjs_ng.cppm` and `src/inline.cpp` are what [tools/gen_exports.py](../tools/gen_exports.py) writes from the pinned header; every declaration is exported, every static inline function adapted, every macro mapped or listed with its reason.
- `python3 tools/run_conformance.py`: QuickJS-ng's own `tests/*.js` on [examples/qjs](../examples/qjs), a host written against the module, with each test's frontmatter honoured as `run-test262` does. 77 pass. Left out are those `tests.conf` excludes and those that import quickjs-libc.
