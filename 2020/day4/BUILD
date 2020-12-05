cc_library(
    name = "day4_lib",
    srcs = ["day4.c", "passport.c", "passport.h"],
    deps = ["//2020/result:result"],
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
