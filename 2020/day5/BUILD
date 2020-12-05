cc_library(
    name = "day5_lib",
    srcs = ["day5.c"],
    deps = ["//2020/result:result"],
)

cc_binary(
    name = "day5",
    deps = [":day5_lib"],
)

cc_test(
    name = "day5_test",
    srcs = ["day5_test.c"],
    deps = [":day5_lib"],
    data = [":input", ":example"],
)

sh_test(
    name = "day5_leaks",
    srcs = ["//2020/result:leaks-test.sh"],
    args = ["2020/day5/day5_test"],
    data = [":day5_test"],
)
