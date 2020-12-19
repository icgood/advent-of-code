# array
cc_library(
    name = "array",
    srcs = ["array.c"],
    hdrs = ["array.h"],
    visibility = ["//visibility:public"],
)
cc_test(
    name = "array_test",
    srcs = ["array_test.c"],
    deps = [":array"],
)
sh_test(
    name = "array_leaks",
    srcs = ["//day_base:leaks-test.sh"],
    args = ["$(location //day_base/struct:array_test)"],
    data = [":array_test"],
)

# bitmap
cc_library(
    name = "bitmap",
    srcs = ["bitmap.c"],
    hdrs = ["bitmap.h"],
    visibility = ["//visibility:public"],
)
cc_test(
    name = "bitmap_test",
    srcs = ["bitmap_test.c"],
    deps = [":bitmap"],
)
sh_test(
    name = "bitmap_leaks",
    srcs = ["//day_base:leaks-test.sh"],
    args = ["$(location //day_base/struct:bitmap_test)"],
    data = [":bitmap_test"],
)

# hashmap
cc_library(
    name = "hashmap",
    srcs = ["hashmap.c"],
    hdrs = ["hashmap.h"],
    deps = [":array"],
    visibility = ["//visibility:public"],
)
cc_test(
    name = "hashmap_test",
    srcs = ["hashmap_test.c"],
    deps = [":hashmap"],
)
sh_test(
    name = "hashmap_leaks",
    srcs = ["//day_base:leaks-test.sh"],
    args = ["$(location //day_base/struct:hashmap_test)"],
    data = [":hashmap_test"],
)

# deque
cc_library(
    name = "deque",
    srcs = ["deque.c"],
    hdrs = ["deque.h"],
    deps = [":array", ":bitmap"],
    visibility = ["//visibility:public"],
)
cc_test(
    name = "deque_test",
    srcs = ["deque_test.c"],
    deps = [":deque"],
)
sh_test(
    name = "deque_leaks",
    srcs = ["//day_base:leaks-test.sh"],
    args = ["$(location //day_base/struct:deque_test)"],
    data = [":deque_test"],
)
