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
        ["ui/*.cpp"],
        exclude = ["ui/*_test.cpp"],
    ),
    hdrs = glob(["ui/**/*.h"]),
    copts = nRF5_copts,
    includes = ["ui/include"],
    linkopts = nRF5_linkopts,
)

cc_test(
    name = "ui_test",
    srcs = glob([
        "ui/*_test.cpp",
        "ui/**/*.h",
    ]),
    includes = ["ui/include"],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
