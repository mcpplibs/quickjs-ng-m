#!/usr/bin/env python3
"""Run QuickJS-ng's own tests (tests/*.js of the release, the set upstream's
tests.conf runs locally) on examples/qjs, a host written against
import mcpplibs.mwrap.quickjs_ng.

The release tarball is downloaded once into target/upstream and checked
against upstream/documents.json; its files are used unchanged. Each test's
frontmatter is honoured as upstream's run-test262 honours it: a `negative`
test must throw its error type, `qjs:track-promise-rejections` makes an
unhandled rejection a failure, `module` / `qjs:no-detect-module` say how the
file is evaluated. Left out: what tests.conf excludes, and the tests that
import quickjs-libc (`qjs:std`, `qjs:os`, `qjs:bjson`), which the engine
package does not include. Every other test must pass.
"""
import argparse
import os
from pathlib import Path
import re
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from inventory import fetch  # noqa: E402


def excluded_by_conf(upstream: Path) -> set[str]:
    out, section = set(), None
    for line in (upstream / 'tests.conf').read_text(encoding='utf-8').splitlines():
        line = line.strip()
        if line.startswith('['):
            section = line
        elif section == '[exclude]' and line:
            out.add(line)
    return out


def frontmatter(text: str) -> dict:
    m = re.search(r'/\*---(.*?)---\*/', text, flags=re.S)
    if not m:
        return {}
    body = m.group(1)
    meta = {'flags': [], 'features': []}
    for key in ('flags', 'features'):
        f = re.search(key + r':\s*\[([^\]]*)\]', body)
        if f:
            meta[key] = [x.strip() for x in f.group(1).split(',') if x.strip()]
    n = re.search(r'negative:.*?type:\s*(\w+)', body, flags=re.S)
    if n:
        meta['negative'] = n.group(1)
    return meta


def host(example: Path, mcpp: str, toolchain: str | None) -> Path:
    command = [mcpp, 'build', '--profile', 'release'] + (['--toolchain', toolchain] if toolchain else [])
    print('+', ' '.join(command), flush=True)
    subprocess.run(command, cwd=example, check=True)
    name = 'qjs.exe' if os.name == 'nt' else 'qjs'
    found = sorted((p for p in (example / 'target').rglob(name) if p.parent.name == 'bin'), key=lambda p: p.stat().st_mtime)
    if not found:
        sys.exit('examples/qjs was not built')
    return found[-1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mcpp', default='mcpp')
    parser.add_argument('--toolchain')
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    upstream = fetch(root)
    qjs = host(root / 'examples/qjs', args.mcpp, args.toolchain)
    conf = excluded_by_conf(upstream)
    passed, failed, libc = [], [], []
    for test in sorted((upstream / 'tests').glob('*.js')):
        rel = test.relative_to(upstream).as_posix()
        if rel in conf:
            continue
        text = test.read_text(encoding='utf-8', errors='replace')
        if re.search(r'''["']qjs:(std|os|bjson)["']''', text):
            libc.append(rel)
            continue
        meta = frontmatter(text)
        options = []
        if 'qjs:track-promise-rejections' in meta['flags'] if meta else False:
            options.append('--track-rejections')
        if meta and 'module' in meta['flags']:
            options.append('--module')
        elif meta and 'qjs:no-detect-module' in meta['flags']:
            options.append('--script')
        run = subprocess.run([str(qjs), *options, rel], cwd=upstream, capture_output=True, text=True, timeout=600)
        output = (run.stderr or '') + (run.stdout or '')
        negative = meta.get('negative') if meta else None
        if negative:
            ok = run.returncode == 1 and re.search(r'\b' + negative + r'\b', run.stderr or '') is not None
        else:
            ok = run.returncode == 0
        (passed if ok else failed).append(rel)
        if not ok:
            print(f'FAIL {rel} (exit {run.returncode}{", expected " + negative if negative else ""})\n{output[-1500:]}', flush=True)
    print(f'{len(passed)} passed, {len(failed)} failed; left out: {len(conf)} by tests.conf, '
          f'{len(libc)} that import quickjs-libc ({", ".join(Path(p).name for p in libc)})')
    if failed or len(passed) < 75:  # 77 run in 0.17.0: a guard against finding none
        sys.exit(1)


if __name__ == '__main__':
    main()
