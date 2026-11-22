# quickjs_ng

> 通过 C++23 模块使用 QuickJS-ng（小巧的可嵌入 JavaScript 引擎）。

[English](README.md) · **简体中文** · [上游](https://github.com/quickjs-ng/quickjs) · [发布](https://github.com/mcpplibs/quickjs-ng-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [索引](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/quickjs-ng-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/quickjs-ng-m/actions/workflows/ci.yml)

上游的 C API 保持不变：`JSRuntime`、`JS_Eval`、`JS_NewObject`、`JS_FreeValue`
就是 `<quickjs.h>` 里的上游全局名字。引擎本身是 `compat.quickjs-ng` 0.17.0。
本包为头文件里的 `static inline` API（`JS_NewInt32`、`JS_IsException`、
`JS_ToCString` …）提供适配，并把宏映射成常量和 constexpr 函数。每次 CI 都会在
一个只用本模块写成的宿主上跑 QuickJS-ng 自带的测试，并且全部通过。

## 模块

- `import mcpplibs.mwrap.quickjs_ng;`：嵌入 API，`mcpplibs::mwrap::quickjs_ng` 里的 static inline API，并重新导出下面这个模块
- `import mcpplibs.mwrap.quickjs_ng.macros;`：`JS_UNDEFINED`、`JS_EVAL_TYPE_MODULE`、`JS_CFUNC_DEF` 等，位于 `mcpplibs::mwrap::quickjs_ng::macros`

> 更多模块 API -> [用法和 API](docs/usage.md)

## 快速开始

```sh
mcpp new scripted --template mcpplibs.mwrap.quickjs_ng@0.17.0.1
cd scripted
mcpp run
```

默认模板注册一个 C++ 函数，执行一段调用它的脚本，再打印结果。

```toml
[dependencies]
mcpplibs.mwrap.quickjs_ng = "0.17.0.1"
```

```cpp
import std;
import mcpplibs.mwrap.quickjs_ng;
using namespace mcpplibs::mwrap::quickjs_ng;          // JS_ToCString、JS_IsException …
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

## static inline API 和宏

`static inline` 函数是内部链接，不能导出。27 个公开的 static inline 函数以原名
适配在 `mcpplibs::mwrap::quickjs_ng` 里，每个都调用上游自己的实现。宏映射成
常量（`JS_PROP_C_W_E`、`JS_TAG_*`、作为 constexpr `JSValue` 的 `JS_UNDEFINED`）
和 constexpr 函数（`JS_MKVAL`、`JS_VALUE_GET_TAG`，以及用于常量初始化属性表的
`JS_CFUNC_DEF`）。`JS_CFUNC_SPECIAL_DEF` 和 `JS_FreeValues` / `JS_FreeAtoms`
仍然是宏，放在 `<mwrap/quickjs_ng/macros.hpp>` 里，在 import 之前包含。支持
64 位目标（结构体形式的 `JSValue`）。详见 [docs/usage.md](docs/usage.md)。

## 示例

- [examples/qjs](examples/qjs)：只用本模块写成，运行脚本或 ES 模块，支持从文件导入和导入属性；
  [tools/run_conformance.py](tools/run_conformance.py) 在它上面跑 QuickJS-ng 的测试。

## 构建和测试

Linux、macOS、Windows 的 CI 构建模块并运行测试、导出审计
（[tools/check_exports.py](tools/check_exports.py)：生成的接口是最新的，每个声明
都已导出，每个宏都已映射或列出原因）以及 QuickJS-ng 自带的 `tests/*.js`。
另有一个作业把默认模板当作已安装的包来生成并运行。

## 许可证

包装部分为 [Apache-2.0](LICENSE)，作者 mcpp-community；QuickJS-ng 本身仍是 MIT
（[upstream/quickjs-ng/LICENSE](upstream/quickjs-ng/LICENSE)）。
