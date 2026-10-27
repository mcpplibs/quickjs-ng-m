// <mwrap/quickjs_ng/macros.hpp> — QuickJS-ng's macros as macros, for the ones a
// function cannot be: JS_CFUNC_SPECIAL_DEF (it pastes tokens into an
// enumerator and a member name), JS_FreeValues / JS_FreeValuesRT /
// JS_FreeAtoms / JS_FreeAtomsRT (each value is freed before the next
// argument is evaluated) with their JS_COUNT_ARGS / JS_CALLX machinery, and
// QUICKJS_NG (a test for #if). Include it before
// `import mcpplibs.mwrap.quickjs_ng;`: it is <quickjs.h>, so every other macro
// is defined too, and while a macro is defined it expands even inside
// `mcpplibs::mwrap::quickjs_ng::macros::NAME`.
#pragma once
#include <quickjs.h>
