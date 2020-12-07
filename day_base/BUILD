cc_library(
    name = "day_result",
    srcs = ["day_result.c"],
    hdrs = ["day_result.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "day_result_run",
    srcs = ["day_result_run.c"],
    deps = [":day_result"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "day_result_test",
    srcs = ["day_result_test.c"],
    deps = [":day_result"],
    visibility = ["//visibility:public"],
)

exports_files(["leaks-test.sh"], ["//visibility:public"])
