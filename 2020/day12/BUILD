load('//day_base:day_cc_solution.bzl', 'day_cc_solution')

cc_library(
    name = 'helpers',
    srcs = ['part1.c', 'part2.c'],
    hdrs = ['day12.h'],
)

day_cc_solution(
    year = '2020',
    day = 'day12',
    deps = [':helpers', '//day_base/struct:array'],
)
