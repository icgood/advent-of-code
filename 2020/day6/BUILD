cc_library(
    name = "sol_lib",
    srcs = ["sol.c"],
    deps = ["//result:result"],
)

cc_binary(
    name = "sol",
    deps = [":sol_lib"],
)

cc_test(
    name = "sol_test",
    srcs = ["sol_test.c"],
    deps = [":sol_lib"],
    data = [":input", ":example"],
)

sh_test(
    name = "sol_leaks",
    srcs = ["//result:leaks-test.sh"],
    args = ["2020/day6/sol_test"],
    data = [":sol_test"],
)
