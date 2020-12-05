cc_library(
    name = "day3_lib",
    srcs = ["day3.c"],
    hdrs = ["day3.h"],
)

cc_binary(
    name = "day3",
    srcs = ["day3_main.c"],
    deps = [":day3_lib"],
)

cc_test(
    name = "day3_test",
    srcs = ["day3_test.c"],
    deps = [":day3_lib"],
    data = [":input", ":example"],
)
