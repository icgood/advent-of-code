cc_library(
    name = "day1_lib",
    srcs = ["day1.c"],
    deps = ["//result:result"],
)

cc_binary(
    name = "day1",
    deps = [":day1_lib"],
)

cc_test(
    name = "day1_test",
    srcs = ["day1_test.c"],
    deps = [":day1_lib"],
    data = [":input", ":example"],
)

sh_test(
    name = "day1_leaks",
    srcs = ["//result:leaks-test.sh"],
    args = ["2015/day1/day1_test"],
    data = [":day1_test"],
)
