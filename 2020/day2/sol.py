import re
import sys

pattern = re.compile(r'^(\d+)-(\d+) (\w): (.*)$')
records = []

for line in sys.stdin:
    match = pattern.match(line)
    assert match is not None
    one, two, letter, password = match.groups()
    records.append((int(one), int(two), letter, password))

# part 1
valid = 0
for min_n, max_n, letter, password in records:
    num = password.count(letter)
    if min_n <= num and num <= max_n:
        valid += 1
print('part 1:', valid)

# part 2
valid = 0
for first, second, letter, password in records:
    found_first = password[first-1] == letter
    found_second = password[second-1] == letter
    if found_first ^ found_second:
        valid += 1
print('part 2:', valid)
