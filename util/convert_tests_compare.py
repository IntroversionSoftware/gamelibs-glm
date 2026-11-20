#!/usr/bin/env python3
"""Turn GLM_CHECK(<comparison>) into the GLM_CHECK_* form that prints both operands on failure.

Second pass after convert_tests.py, again line-level and conservative (anything it can't parse
with confidence stays a plain GLM_CHECK):

- GLM_CHECK(glm::all(glm::equal(a, b, eps)))  -> GLM_CHECK_NEAR(a, b, eps)
- GLM_CHECK(glm::equal(a, b, eps))            -> GLM_CHECK_NEAR(a, b, eps)   (scalar form)
- GLM_CHECK(a == b)                           -> GLM_CHECK_EQ(a, b), likewise != < <= > >=
- GLM_CHECK(true == x) / GLM_CHECK(x == true) -> GLM_CHECK(x)

glm::epsilonEqual is left alone: it tests |a - b| < eps, where glm::equal tests <=.

Operands with a top-level comma (template arguments) are parenthesized for the macro. A check
counts the same either way, so a test's check count must not change.

Usage: convert_tests_compare.py [--dry-run] file.cpp...
"""

import re
import sys

CHECK = re.compile(r'^(?P<head>.*?)GLM_CHECK\((?P<body>.*)\);(?P<tail>[ \t]*(?://.*)?)$')
MACROS = {'==': 'EQ', '!=': 'NE', '<': 'LT', '<=': 'LE', '>': 'GT', '>=': 'GE'}


def mask_strings(text):
    """Same length as text, with string and character literal contents blanked out."""
    return re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', lambda m: '"' + ' ' * (len(m.group(0)) - 2) + '"', text)


def depths(text):
    """Bracket depth ((), [], {}) before each character; None if unbalanced."""
    masked = mask_strings(text)
    out = []
    depth = 0
    for c in masked:
        if c in ')]}':
            depth -= 1
            if depth < 0:
                return None
        out.append(depth)
        if c in '([{':
            depth += 1
    return out if depth == 0 else None


def operators(text, depth):
    """Top-level (depth 0) comparison/angle tokens as (position, token)."""
    masked = mask_strings(text)
    found = []
    i = 0
    while i < len(masked):
        if depth[i] == 0:
            two = masked[i:i + 2]
            if two in ('==', '!=', '<=', '>=', '&&', '||', '<<', '>>', '->'):
                found.append((i, two))
                i += 2
                continue
            if masked[i] in '<>?=!':
                found.append((i, masked[i]))
        i += 1
    return found


def angles_balanced(text):
    masked = mask_strings(text)
    masked = re.sub(r'<<|>>|->|<=|>=', '  ', masked)
    return masked.count('<') == masked.count('>')


def split_args(text):
    """Split at depth-0 commas; None if unbalanced or if a piece has unbalanced angle brackets."""
    depth = depths(text)
    if depth is None:
        return None
    masked = mask_strings(text)
    parts = []
    start = 0
    for i, c in enumerate(masked):
        if c == ',' and depth[i] == 0:
            parts.append(text[start:i].strip())
            start = i + 1
    parts.append(text[start:].strip())
    if any(not p or not angles_balanced(p) for p in parts):
        return None
    return parts


def wrap(operand):
    """Parenthesize an operand that has a top-level comma, so it stays one macro argument."""
    depth = depths(operand)
    masked = mask_strings(operand)
    if any(c == ',' and depth[i] == 0 for i, c in enumerate(masked)):
        return '(' + operand + ')'
    return operand


def call_args(body, name):
    """If body is exactly name(...), the text inside the parentheses."""
    if not body.startswith(name + '(') or not body.endswith(')'):
        return None
    inner = body[len(name) + 1:-1]
    return inner if depths(inner) is not None else None


def convert_body(body):
    body = body.strip()
    depth = depths(body)
    if depth is None:
        return None

    inner = call_args(body, 'glm::all')
    if inner is not None:
        inner = call_args(inner.strip(), 'glm::equal')
    else:
        inner = call_args(body, 'glm::equal')
    if inner is not None:
        args = split_args(inner)
        if args and len(args) == 3:
            return 'NEAR', args
        return None

    ops = operators(body, depth)
    if len(ops) != 1 or ops[0][1] not in MACROS:
        return None
    pos, op = ops[0]
    a, b = body[:pos].strip(), body[pos + len(op):].strip()
    if not a or not b or not angles_balanced(a) or not angles_balanced(b):
        return None
    if op == '==' and a == 'true':
        return 'CHECK', [b]
    if op == '==' and b == 'true':
        return 'CHECK', [a]
    return MACROS[op], [a, b]


def convert(text):
    nl = '\r\n' if '\r\n' in text else '\n'
    lines = text.split(nl)
    stats = {}
    for i, line in enumerate(lines):
        m = CHECK.match(line)
        if not m:
            continue
        result = convert_body(m.group('body'))
        if result is None:
            stats['unchanged'] = stats.get('unchanged', 0) + 1
            continue
        kind, args = result
        macro = 'GLM_CHECK' if kind == 'CHECK' else 'GLM_CHECK_' + kind
        lines[i] = m.group('head') + macro + '(' + ', '.join(wrap(a) for a in args) + ');' + m.group('tail')
        stats[kind] = stats.get(kind, 0) + 1
    return nl.join(lines), stats


if __name__ == '__main__':
    dry = '--dry-run' in sys.argv
    totals = {}
    for path in [a for a in sys.argv[1:] if a != '--dry-run']:
        text = open(path, encoding='utf-8', newline='').read()
        new, stats = convert(text)
        for k, v in stats.items():
            totals[k] = totals.get(k, 0) + v
        if not dry and new != text:
            open(path, 'w', encoding='utf-8', newline='').write(new)
        print(f'{path}: {stats}')
    print(f'total: {totals}')
