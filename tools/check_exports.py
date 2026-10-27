#!/usr/bin/env python3
"""Audit the module surface against quickjs.h (the pinned release's).

- src/quickjs_ng.cppm and src/inline.cpp are what tools/gen_exports.py
  writes: every type, enumerator of a named enum and JS_EXTERN function
  exported, every public static inline function adapted.
- Every macro of quickjs.h is mapped in src/macros.cppm, kept as a macro in
  <mwrap/quickjs_ng/macros.hpp> (listed below with the reason a function
  cannot be it), or excluded below with a reason; every JS_TAG_* enumerator
  of the unnamed enum is mapped.
"""
from pathlib import Path
import re
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from inventory import fetch, surface  # noqa: E402

SIDE_HEADER = {
    'JS_CFUNC_SPECIAL_DEF': 'pastes its argument into an enumerator and a member name',
    'JS_FreeValues': 'frees each value before the next argument is evaluated',
    'JS_FreeValuesRT': 'frees each value before the next argument is evaluated',
    'JS_FreeAtoms': 'frees each atom before the next argument is evaluated',
    'JS_FreeAtomsRT': 'frees each atom before the next argument is evaluated',
    'JS_COUNT_ARGS': "the machinery of JS_FreeValues / JS_FreeAtoms",
    'JS_COUNT_ARGS_': "the machinery of JS_FreeValues / JS_FreeAtoms",
    'JS_CALLX_': "the machinery of JS_FreeValues / JS_FreeAtoms",
    'JS_CALLX__': "the machinery of JS_FreeValues / JS_FreeAtoms",
    **{f'JS_CALLX{i}': "the machinery of JS_FreeValues / JS_FreeAtoms" for i in range(1, 9)},
    'QUICKJS_NG': 'a test for #if',
}

EXCLUDED = {
    'QUICKJS_H': 'include guard',
    'QUICKJS_NG_PLAT_WIN32': "the header's own platform test",
    'QUICKJS_NG_CC_GNULIKE': "the header's own compiler test",
    'JS_EXTERN': 'a linkage annotation of declarations',
    'JS_LIBC_EXTERN': 'a linkage annotation of declarations',
    'JS_MODULE_EXTERN': 'a linkage annotation of declarations',
    'JS_PRINTF_FORMAT': 'a format-checking annotation of declarations',
    'JS_PRINTF_FORMAT_ATTR': 'a format-checking annotation of declarations',
    'JS_FLOAT64_TAG_ADDEND': 'the NaN-boxed representation only (32-bit targets, refused)',
}


def mapped_names(text: str) -> set[str]:
    text = text[text.index('export module'):]
    names = set(re.findall(r'inline constexpr (?:[\w:&]+\s+)+?(\w+)\s*=', text))
    names |= set(re.findall(r'^constexpr\s+[\w:\*\s]+?\b(\w+)\s*\(', text, flags=re.M))
    names |= set(re.findall(r'^using\s+(\w+)\s*=', text, flags=re.M))
    return names


def main():
    root = Path(__file__).resolve().parents[1]
    problems = []
    generated = subprocess.run([sys.executable, str(root / 'tools/gen_exports.py'), '--check'], capture_output=True, text=True)
    if generated.returncode != 0:
        problems.append(generated.stderr.strip() or generated.stdout.strip())
    sf = surface(fetch(root) / 'quickjs.h')
    exported = set(re.findall(r'export using ::(\w+);', (root / 'src/quickjs_ng.cppm').read_text(encoding='utf-8')))
    missing = (set(sf.types) | set(sf.enumerators) | set(sf.functions)) - exported
    if missing:
        problems.append('src/quickjs_ng.cppm lacks: ' + ', '.join(sorted(missing)))
    adapted = set(re.findall(r'^[\w\s\*]+?\b(JS_\w+)\(', (root / 'src/quickjs_ng.cppm').read_text(encoding='utf-8'), flags=re.M))
    missing = {i.name for i in sf.inlines} - adapted
    if missing:
        problems.append('static inline functions not adapted: ' + ', '.join(sorted(missing)))
    mapped = mapped_names((root / 'src/macros.cppm').read_text(encoding='utf-8'))
    macros = set(sf.macros)
    unaccounted = macros - mapped - SIDE_HEADER.keys() - EXCLUDED.keys()
    if unaccounted:
        problems.append('macros neither mapped, side-header nor excluded: ' + ', '.join(sorted(unaccounted)))
    both = (SIDE_HEADER.keys() | EXCLUDED.keys()) & mapped
    if both:
        problems.append('listed as unmapped yet mapped: ' + ', '.join(sorted(both)))
    stale = (SIDE_HEADER.keys() | EXCLUDED.keys()) - macros
    if stale:
        problems.append('listed but not in quickjs.h: ' + ', '.join(sorted(stale)))
    missing = set(sf.tag_constants) - mapped
    if missing:
        problems.append('JS_TAG_* not mapped: ' + ', '.join(sorted(missing)))
    if problems:
        sys.exit('\n'.join(problems))
    print(f'{len(sf.types)} types, {len(sf.enumerators)} enumerators and {len(sf.functions)} functions exported; '
          f'{len(sf.inlines)} static inline functions adapted; {len(macros)} macros: {len(macros & mapped)} mapped, '
          f'{len(SIDE_HEADER)} kept as macros, {len(EXCLUDED)} excluded; {len(sf.tag_constants)} JS_TAG_* mapped')


if __name__ == '__main__':
    main()
