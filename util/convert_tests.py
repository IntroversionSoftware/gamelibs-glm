#!/usr/bin/env python3
"""Convert GLM test files to the glm_test.hpp harness (test/common).

Mechanical, line-level rewrites only; anything ambiguous is left alone for a person to convert:

- `#include "glm_test.hpp"` is added before the first #include that isn't inside a preprocessor
  conditional (or at the top of the file if there's none).
- `Error += <cond> ? 0 : 1;` becomes `Error += GLM_CHECK(<cond>);`, and `Error += <cond> ? 1 : 0;`
  becomes `Error += GLM_CHECK(!(<cond>));`, unless <cond> contains another `?`.
- Inside main(): `Error += <call>;` becomes `Error += GLM_TEST(<call>);` when <call> is a plain
  function call, and the final `return Error;` becomes `return GLM_TEST_FINISH(Error);`.

Usage: convert_tests.py [--dry-run] file.cpp...   (prints a per-file summary)
"""

import re
import sys

CHECK = re.compile(r'^(?P<indent>[ \t]*)Error\s*\+=\s*(?P<cond>.+?)\s*\?\s*(?P<pass>[01])\s*:\s*(?P<fail>[01])\s*;(?P<tail>[ \t]*(?://.*)?)$')
CALL = re.compile(r'^(?P<indent>[ \t]*)Error\s*\+=\s*(?P<call>(?:::)?[A-Za-z_][\w:]*(?:<[^;]*>)?\s*\(.*\))\s*;(?P<tail>[ \t]*(?://.*)?)$')
RETURN = re.compile(r'^(?P<indent>[ \t]*)return\s+Error\s*;(?P<tail>[ \t]*(?://.*)?)$')
MAIN = re.compile(r'^\s*int\s+main\s*\(')


def strip_strings(text):
    return re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', '""', text)


def balanced(text):
    depth = 0
    for c in strip_strings(text):
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
            if depth < 0:
                return False
    return depth == 0


def main_ranges(lines):
    """Line index ranges [start, end] of each `int main(...)` body."""
    ranges = []
    i = 0
    while i < len(lines):
        if MAIN.match(lines[i]):
            depth = 0
            started = False
            for j in range(i, len(lines)):
                code = strip_strings(lines[j].split('//')[0])
                depth += code.count('{') - code.count('}')
                started = started or '{' in code
                if started and depth == 0:
                    ranges.append((i, j))
                    i = j
                    break
        i += 1
    return ranges


def include_position(lines):
    depth = 0
    for i, line in enumerate(lines):
        s = line.strip()
        if re.match(r'#\s*if', s):
            depth += 1
        elif re.match(r'#\s*endif', s):
            depth -= 1
        elif re.match(r'#\s*include\b', s) and depth == 0:
            return i
    return 0


def convert(text):
    nl = '\r\n' if '\r\n' in text else '\n'
    lines = text.split(nl)
    stats = {'checks': 0, 'inverted': 0, 'tests': 0, 'finish': 0, 'skipped_nested': 0}
    mains = main_ranges(lines)

    def in_main(i):
        return any(a <= i <= b for a, b in mains)

    last_return_in_main = {}
    for a, b in mains:
        for i in range(b, a - 1, -1):
            if RETURN.match(lines[i]):
                last_return_in_main[(a, b)] = i
                break

    out = []
    for i, line in enumerate(lines):
        m = CHECK.match(line)
        if m and m.group('pass') != m.group('fail'):
            cond = m.group('cond')
            if '?' in strip_strings(cond) or not balanced(cond):
                stats['skipped_nested'] += 1
            else:
                if m.group('pass') == '0':
                    new = 'GLM_CHECK(' + cond + ')'
                    stats['checks'] += 1
                else:
                    new = 'GLM_CHECK(!(' + cond + '))'
                    stats['inverted'] += 1
                line = m.group('indent') + 'Error += ' + new + ';' + m.group('tail')
        elif in_main(i):
            c = CALL.match(line)
            if c and 'GLM_' not in c.group('call') and balanced(c.group('call')):
                line = c.group('indent') + 'Error += GLM_TEST(' + c.group('call') + ');' + c.group('tail')
                stats['tests'] += 1
            elif i in last_return_in_main.values():
                r = RETURN.match(line)
                line = r.group('indent') + 'return GLM_TEST_FINISH(Error);' + r.group('tail')
                stats['finish'] += 1
        out.append(line)

    if 'glm_test.hpp' not in text:
        pos = include_position(out)
        out.insert(pos, '#include "glm_test.hpp"')
    return nl.join(out), stats


if __name__ == '__main__':
    args = [a for a in sys.argv[1:] if a != '--dry-run']
    dry = '--dry-run' in sys.argv
    totals = {}
    for path in args:
        text = open(path, encoding='utf-8', newline='').read()
        new, stats = convert(text)
        for k, v in stats.items():
            totals[k] = totals.get(k, 0) + v
        if not dry and new != text:
            open(path, 'w', encoding='utf-8', newline='').write(new)
        print(f'{path}: {stats}')
    print(f'total: {totals}')
