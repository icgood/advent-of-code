cc_library(
    name = "day4_lib",
    srcs = ["day4.c", "passport.c", "passport.h"],
    deps = ["//result:result"],
)

cc_binary(
    name = "day4",
    deps = [":day4_lib"],
)

cc_test(
    name = "day4_test",
    srcs = ["day4_test.c"],
    deps = [":day4_lib"],
    data = [":input", ":example"],
)

sh_test(
    name = "day4_leaks",
    srcs = ["//result:leaks-test.sh"],
    args = ["2020/day4/day4_test"],
    data = [":day4_test"],
)
