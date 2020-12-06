cc_library(
    name = "day3_lib",
    srcs = ["day3.c"],
    deps = ["//result:result"]
)

cc_binary(
    name = "day3",
    deps = [":day3_lib"],
)

cc_test(
    name = "day3_test",
    srcs = ["day3_test.c"],
    deps = [":day3_lib"],
    data = [":input", ":example"],
)

sh_test(
    name = "day3_leaks",
    srcs = ["//result:leaks-test.sh"],
    args = ["2020/day3/day3_test"],
    data = [":day3_test"],
)
