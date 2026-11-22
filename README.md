# quickjs_ng

> QuickJS-ng, the small embeddable JavaScript engine, through C++23 modules.

**English** · [简体中文](README.zh.md) · [Upstream](https://github.com/quickjs-ng/quickjs) · [Releases](https://github.com/mcpplibs/quickjs-ng-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/quickjs-ng-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/quickjs-ng-m/actions/workflows/ci.yml)

The upstream C API is unchanged: `JSRuntime`, `JS_Eval`, `JS_NewObject`,
`JS_FreeValue` are the upstream globals of `<quickjs.h>`. The engine is
`compat.quickjs-ng` 0.17.0. This package adapts the header's `static inline`
API (`JS_NewInt32`, `JS_IsException`, `JS_ToCString` …) and maps its macros to
constants and constexpr functions. QuickJS-ng's own tests pass on every CI
run, on a host written against the module.

## Modules

- `import mcpplibs.mwrap.quickjs_ng;`: the embedding API, the static inline API in `mcpplibs::mwrap::quickjs_ng`, re-exporting the one below
- `import mcpplibs.mwrap.quickjs_ng.macros;`: `JS_UNDEFINED`, `JS_EVAL_TYPE_MODULE`, `JS_CFUNC_DEF` … in `mcpplibs::mwrap::quickjs_ng::macros`

> More Modules API -> [Usage and API](docs/usage.md)

## Getting started

```sh
mcpp new scripted --template mcpplibs.mwrap.quickjs_ng@0.17.0.1
cd scripted
mcpp run
```

The default template registers a C++ function, evaluates a script that calls
it, and prints the result.

```toml
[dependencies]
mcpplibs.mwrap.quickjs_ng = "0.17.0.1"
```

```cpp
import std;
import mcpplibs.mwrap.quickjs_ng;
using namespace mcpplibs::mwrap::quickjs_ng;          // JS_ToCString, JS_IsException …
using namespace mcpplibs::mwrap::quickjs_ng::macros;  // JS_EVAL_TYPE_GLOBAL …

int main() {
    JSRuntime* rt = JS_NewRuntime();
    JSContext* ctx = JS_NewContext(rt);
    std::string_view code = "[1, 2, 3].map(x => x * x).join(' ')";
    JSValue v = JS_Eval(ctx, code.data(), code.size(), "<main>", JS_EVAL_TYPE_GLOBAL);
    const char* s = JS_ToCString(ctx, v);
    std::println("{}", s);
    JS_FreeCString(ctx, s);
    JS_FreeValue(ctx, v);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
}
```

## The static inline API and macros

`static inline` functions have internal linkage and cannot be exported. The 27
public ones are adapted under their names in `mcpplibs::mwrap::quickjs_ng`,
each calling upstream's body. Macros are constants (`JS_PROP_C_W_E`, `JS_TAG_*`,
`JS_UNDEFINED` as a constexpr `JSValue`) and constexpr functions (`JS_MKVAL`,
`JS_VALUE_GET_TAG`, `JS_CFUNC_DEF` for constant-initialised property tables).
`JS_CFUNC_SPECIAL_DEF` and `JS_FreeValues` / `JS_FreeAtoms` stay macros in
`<mwrap/quickjs_ng/macros.hpp>`, included before the import. 64-bit targets
(the struct `JSValue`) are supported. Details: [docs/usage.md](docs/usage.md).

## Examples

- [examples/qjs](examples/qjs): runs a script or an ES module, with file imports and import attributes,
  written against the module; [tools/run_conformance.py](tools/run_conformance.py) runs QuickJS-ng's tests on it.

## Build and testing

Linux, macOS and Windows CI build the modules and run the tests, the export
audit ([tools/check_exports.py](tools/check_exports.py): the generated surface is
fresh, every declaration exported, every macro mapped or listed with its reason)
and QuickJS-ng's own `tests/*.js`. A separate job scaffolds the default template
as an installed package and runs it.

## License

[Apache-2.0](LICENSE) for the wrapper, by mcpp-community; QuickJS-ng remains MIT
([upstream/quickjs-ng/LICENSE](upstream/quickjs-ng/LICENSE)).
