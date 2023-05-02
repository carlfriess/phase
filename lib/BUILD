package(default_visibility = ["//visibility:public"])

load("@//external/nRF5:nRF5.bzl", "nRF5_copts", "nRF5_linkopts")
load("@//tools/img:embed.bzl", "embed_mask")

cc_library(
    name = "GC9A01",
    srcs = glob(["GC9A01/*.c"]),
    hdrs = glob(["GC9A01/*.h"]),
    copts = nRF5_copts,
    includes = ["GC9A01"],
    linkopts = nRF5_linkopts,
)

embed_mask(
    name = "app_icon_mask",
    img = "ui/watch/app_icon_mask.png",
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
        ":utf8",
        "@fontem//:libfontem",
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
    ) + [
        ":app_icon_mask",
    ],
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
    copts = ["-fshort-enums"],
    deps = [
        ":ui",
        ":ui-watch",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "utf8",
    srcs = [
        "utf8/decode.h",
        "utf8/utf8.cpp",
    ],
    hdrs = ["utf8/include/utf8.h"],
    copts = nRF5_copts,
    includes = ["utf8/include"],
    linkopts = nRF5_linkopts,
)

cc_test(
    name = "utf8_test",
    srcs = ["utf8/utf8_test.cpp"],
    copts = ["-fshort-enums"],
    deps = [
        ":utf8",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
