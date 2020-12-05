cc_library(
    name = "day4_lib",
    srcs = ["day4.c", "passport.c", "passport.h"],
    hdrs = ["day4.h"],
)

cc_binary(
    name = "day4",
    srcs = ["day4_main.c"],
    deps = [":day4_lib"],
)

cc_test(
    name = "day4_test",
    srcs = ["day4_test.c"],
    deps = [":day4_lib"],
    data = [":input", ":example"],
)
