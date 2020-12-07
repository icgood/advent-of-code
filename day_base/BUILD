cc_library(
    name = "day_result",
    hdrs = ["day_result.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "day_run",
    srcs = ["day_run.c"],
    deps = [":day_result"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "day_check",
    srcs = ["day_check.c"],
    deps = [":day_result"],
    visibility = ["//visibility:public"],
)

exports_files(["leaks-test.sh"], ["//visibility:public"])
