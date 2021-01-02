load("@//external/nRF5:nRF5.bzl", "nRF5_copts", "nRF5_linkopts")

cc_library(
    name = "libfontem",
    srcs = [
        "src/fontrender.c",
    ],
    hdrs = [
        "src/resource/config.h",
        "src/resource/fontem.h",
    ],
    copts = nRF5_copts,
    includes = [
        "src/resource",
    ],
    linkopts = nRF5_linkopts,
    visibility = ["//visibility:public"],
)
