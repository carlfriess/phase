package(default_visibility = ["//visibility:public"])

load("@//external/nRF5:nRF5.bzl", "nRF5_copts", "nRF5_linkopts")

cc_library(
    name = "GC9A01",
    srcs = glob(["GC9A01/*.c"]),
    hdrs = glob(["GC9A01/*.h"]),
    copts = nRF5_copts,
    includes = ["GC9A01"],
    linkopts = nRF5_linkopts,
)

cc_library(
    name = "ui",
    srcs = glob(
        [
            "ui/*.cpp",
            "ui/*.h",
        ],
        exclude = ["ui/*_test.cpp"],
    ),
    hdrs = ["ui/include/phase-ui.h"],
    copts = nRF5_copts,
    includes = ["ui/include"],
    linkopts = nRF5_linkopts,
    deps = [
        "@fontem//:libfontem",
        "@utfcpp",
    ],
)

cc_library(
    name = "ui-watch",
    srcs = glob(
        [
            "ui/watch/*.cpp",
            "ui/watch/*.h",
        ],
        exclude = ["ui/watch/*_test.cpp"],
    ),
    hdrs = ["ui/include/phase-ui-watch.h"],
    copts = nRF5_copts,
    includes = ["ui/include"],
    linkopts = nRF5_linkopts,
    deps = [
        ":ui",
    ],
)

cc_test(
    name = "ui_test",
    srcs = glob(["ui/**/*_test.cpp"]),
    deps = [
        ":ui",
        ":ui-watch",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
