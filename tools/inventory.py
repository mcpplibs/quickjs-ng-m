#!/usr/bin/env python3
"""The public surface of QuickJS-ng's quickjs.h, from the pinned release.

Shared by tools/gen_exports.py (which writes the export list and the
adaptations of the `static inline` API) and tools/check_exports.py (which
audits the module against it). The header comes from the upstream tarball,
downloaded once into target/upstream and checked against
upstream/documents.json: the same bytes compat.quickjs-ng builds.

The value representation (lines between `#if defined(JS_CHECK_JSVALUE)` and
`#endif /* !JS_NAN_BOXING */`) differs by target; its macros and inline
helpers are the macros module's, written for the struct representation every
64-bit target uses.
"""
from dataclasses import dataclass, field
import hashlib
import json
from pathlib import Path
import re
import sys
import tarfile
import urllib.request


def fetch(root: Path) -> Path:
    pins = json.loads((root / 'upstream/documents.json').read_text(encoding='utf-8'))
    cache = root / 'target/upstream'
    cache.mkdir(parents=True, exist_ok=True)
    archive = cache / 'quickjs-ng-release.tar.gz'
    if not archive.exists() or hashlib.sha256(archive.read_bytes()).hexdigest() != pins['upstream_sha256']:
        print('+ fetch', pins['upstream'], flush=True)
        with urllib.request.urlopen(pins['upstream'], timeout=120) as response:
            archive.write_bytes(response.read())
    digest = hashlib.sha256(archive.read_bytes()).hexdigest()
    if digest != pins['upstream_sha256']:
        sys.exit(f'upstream archive checksum {digest} != {pins["upstream_sha256"]}')
    with tarfile.open(archive) as tar:
        top = tar.getmembers()[0].name.split('/')[0]
        if not (cache / top).exists():
            tar.extractall(cache, filter='data')
    return cache / top


@dataclass
class Inline:
    name: str
    returns: str
    params: str       # as declared
    args: list[str]   # the parameter names, to forward


@dataclass
class Surface:
    types: list[str] = field(default_factory=list)
    enumerators: list[str] = field(default_factory=list)       # of named enums: exported as they are
    tag_constants: list[str] = field(default_factory=list)     # of the unnamed JS_TAG_* enum: constants in the macros module
    functions: list[str] = field(default_factory=list)         # JS_EXTERN
    inlines: list[Inline] = field(default_factory=list)        # static inline, outside the value representation
    macros: list[str] = field(default_factory=list)


def strip(text: str) -> str:
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    return re.sub(r'//[^\n]*', '', text)


def surface(header: Path) -> Surface:
    raw = header.read_text(encoding='utf-8')
    start = raw.index('#if defined(JS_CHECK_JSVALUE)')
    end = raw.index('#endif /* !JS_NAN_BOXING */')
    # Declarations only: the preprocessor's lines (#define JS_EXTERN …) are not.
    outside = re.sub(r'^\s*#[^\n]*(?:\\\n[^\n]*)*', '', strip(raw[:start] + raw[end:]), flags=re.M)
    s = Surface()
    names = set()
    names |= set(re.findall(r'typedef\s+(?:struct|union)\s+(\w+)\s+\1\s*;', outside))
    names |= set(re.findall(r'\}\s*(JS\w+)\s*;', outside))
    names |= set(re.findall(r'typedef\s+[\w\s\*]+?\b(JS\w+)\s*;', outside))
    names |= set(re.findall(r'typedef\s+[\w\s\*]+?\(?\s*\*?\s*\b(JS\w+)\s*\)?\s*\(', outside))
    names |= {'JSValue', 'JSValueUnion'}  # the struct representation's
    for m in re.finditer(r'typedef\s+enum\s+(\w+)\s*\{(.*?)\}\s*\1\s*;', outside, flags=re.S):
        names.add(m.group(1))
        s.enumerators += re.findall(r'\b([A-Z][A-Za-z0-9_]+)\b\s*(?:=[^,]*)?,?', m.group(2))
    for m in re.finditer(r'(?<!typedef\s)enum\s*\{(.*?)\}\s*;', outside, flags=re.S):
        s.tag_constants += re.findall(r'\b(JS_TAG_\w+)\b\s*=', m.group(1))
    s.types = sorted(names)
    # The declared name: the identifier before the parameter list (the
    # trailing JS_PRINTF_FORMAT_ATTR(…) is an attribute, not a declaration).
    plain = re.sub(r'\bJS_PRINTF_FORMAT_ATTR\s*\([^)]*\)|\bJS_PRINTF_FORMAT\b', '', outside)
    s.functions = sorted(set(re.findall(r'\bJS_EXTERN\b[^;{]*?\b(\w+)\s*\(', plain)))
    for m in re.finditer(r'static\s+inline\s+([\w\s\*]+?)\b(JS_\w+)\s*\(([^)]*)\)\s*\{', outside):
        params = ' '.join(m.group(3).split())
        args = [] if params in ('', 'void') else [re.findall(r'(\w+)\s*$', p.strip())[0] for p in params.split(',')]
        s.inlines.append(Inline(m.group(2), ' '.join(m.group(1).split()), params, args))
    s.macros = sorted(set(re.findall(r'^\s*#\s*define\s+(\w+)', raw, flags=re.M)))
    return s


if __name__ == '__main__':
    root = Path(__file__).resolve().parents[1]
    sf = surface(fetch(root) / 'quickjs.h')
    print(f'{len(sf.types)} types, {len(sf.enumerators)} enumerators, {len(sf.tag_constants)} JS_TAG_*, '
          f'{len(sf.functions)} functions, {len(sf.inlines)} static inline, {len(sf.macros)} macros')
    print('inline:', ' '.join(i.name for i in sf.inlines))
    print('types:', ' '.join(sf.types))
