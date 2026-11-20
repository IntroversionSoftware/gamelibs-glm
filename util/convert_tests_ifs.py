#!/usr/bin/env python3
"""Turn `if (failed) { print...; Error++; }` blocks into harness checks.

Third pass after convert_tests.py, for tests that report a failure by printing and then counting
it. A block is converted only when its condition fits on the `if` line and its body is nothing
but output (std::cout/std::printf/print_* helpers) and one `Error++;` / `++Error;` / `Error += 1;`:

	if (!glm::all(glm::equal(Q, P, Epsilon))) {        // or with the brace on the next line
		std::cout << "Error: Q should equal P" << std::endl;
		print_quat("Q", Q);
		Error++;
	}

becomes

	Error += GLM_CHECK(glm::all(glm::equal(Q, P, Epsilon))); // Q should equal P

(the message, when it's a single string literal, is kept as a comment; the harness prints the
expression and location, and convert_tests_compare.py then gives comparisons their value-printing
form). A condition `!x` becomes GLM_CHECK(x), anything else GLM_CHECK(!(cond)). Blocks with an
else, other statements, or no error count are left alone.

Usage: convert_tests_ifs.py [--dry-run] file.cpp...
"""

import re
import sys

sys.path.insert(0, __file__.rsplit('/', 1)[0] if '/' in __file__ else '.')
from convert_tests_compare import convert_body, depths, operators  # noqa: E402

IF = re.compile(r'^(?P<indent>[ \t]*)if\s*\((?P<rest>.*)$')
COUNT = re.compile(r'^\s*(?:Error\s*\+\+|\+\+\s*Error|Error\s*\+=\s*1)\s*;\s*$')
OUTPUT = re.compile(r'^\s*(?:std::(?:cout|cerr|wcout)\s*<<.*;|std::w?printf\s*\(.*\);|print_\w+\s*\(.*\);|std::fflush\(.*\);)\s*$')
MESSAGE = re.compile(r'^\s*std::cout\s*<<\s*"(?:Error|FAIL|Failed)?:?\s*(?P<text>[^"\\]*)"')
LABEL = re.compile(r'^\s*print_\w+_comparison\s*\(.*"(?P<text>[^"\\]*)"\s*\)\s*;\s*$')


def split_condition(rest):
    """`cond) {` or `cond)` -> (cond, has_brace); None if the condition doesn't close on this line."""
    depth = 1
    for i, c in enumerate(rest):
        if c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
            if depth == 0:
                cond, tail = rest[:i].strip(), rest[i + 1:].strip()
                if tail == '{':
                    return cond, True
                if tail == '':
                    return cond, False
                return None
    return None


def single_operand(text):
    """A name, member access or call chain with no operator outside its brackets."""
    d = depths(text)
    if d is None or not text:
        return False
    return not any(depth == 0 and (c.isspace() or c in '=<>!&|?+-*/%^,~') for c, depth in zip(text, d))


def negate(cond):
    # a != b <-> a == b exactly (NaN included); the ordered operators aren't inverted, since
    # !(a > b) and a <= b differ for NaN.
    d = depths(cond)
    ops = operators(cond, d) if d is not None else []
    if len(ops) == 1 and ops[0][1] in ('!=', '=='):
        pos, op = ops[0]
        return cond[:pos].strip() + (' == ' if op == '!=' else ' != ') + cond[pos + 2:].strip()
    if cond.startswith('!') and not cond.startswith('!='):
        inner = cond[1:].strip()
        if single_operand(inner):
            if inner.startswith('(') and inner.endswith(')') and depths(inner[1:-1]) is not None:
                inner = inner[1:-1].strip()
            return inner
    if single_operand(cond):
        return '!' + cond
    return '!(' + cond + ')'


def convert(text):
    nl = '\r\n' if '\r\n' in text else '\n'
    lines = text.split(nl)
    out = []
    stats = {'converted': 0}
    i = 0
    while i < len(lines):
        m = IF.match(lines[i])
        parsed = split_condition(m.group('rest')) if m else None
        if parsed is None:
            out.append(lines[i])
            i += 1
            continue
        cond, brace = parsed
        j = i + 1
        if not brace:
            if j < len(lines) and lines[j].strip() == '{':
                j += 1
            else:
                out.append(lines[i])
                i += 1
                continue
        body = []
        while j < len(lines) and lines[j].strip() != '}':
            body.append(lines[j])
            j += 1
        end = j
        following = lines[end + 1].strip() if end + 1 < len(lines) else ''
        statements = []  # body lines joined into statements, for output spread over several lines
        pending = ''
        for b in body:
            if b.strip():
                pending = (pending + ' ' + b.strip()).strip()
                if pending.endswith(';'):
                    statements.append(pending)
                    pending = ''
        counts = [s for s in statements if COUNT.match(s)]
        others = [s for s in statements if not COUNT.match(s) and not OUTPUT.match(s)]
        if end >= len(lines) or pending or len(counts) != 1 or others or following.startswith('else') or depths(cond) is None:
            out.append(lines[i])
            i += 1
            continue
        check = negate(cond)
        # A block that printed values is converted only when the check prints values too (the
        # GLM_CHECK_* form convert_tests_compare.py will give it), so a failure doesn't say less.
        prints_values = any(
            not COUNT.match(s) and not re.match(r'^std::(cout|cerr|wcout)\s*<<\s*"[^"]*"\s*(<<\s*std::endl\s*)?;$', s)
            and not re.match(r'^std::w?printf\s*\(\s*"[^"%]*"\s*\)\s*;$', s)
            for s in statements)
        if prints_values:
            result = convert_body(check)
            if result is None or result[0] == 'CHECK':
                out.append(lines[i])
                i += 1
                continue
        message = ''
        for s in statements:
            mm = MESSAGE.match(s) or LABEL.match(s)
            text = mm.group('text').strip() if mm else ''
            text = re.sub(r'([,.] *(expected|got)\b.*|:)$', '', text, flags=re.I).strip()  # values follow; the check prints them
            if text and re.sub(r'\s', '', text) != re.sub(r'\s', '', cond):  # not just the condition again
                message = ' // ' + text
                break
        out.append(m.group("indent") + "Error += GLM_CHECK(" + check + ");" + message)
        stats['converted'] += 1
        i = end + 1
    return nl.join(out), stats


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
        if stats['converted']:
            print(f'{path}: {stats}')
    print(f'total: {totals}')
