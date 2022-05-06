load("@//external/nRF5:nRF5.bzl", "nRF5_copts", "nRF5_linkopts")

cc_library(
    name = "bmi270",
    srcs = [
        "bmi2.c",
        "bmi270.c",
    ],
    hdrs = [
        "bmi2.h",
        "bmi270.h",
        "bmi2_defs.h",
    ],
    copts = nRF5_copts,
    includes = [
        ".",
    ],
    linkopts = nRF5_linkopts,
    visibility = ["//visibility:public"],
)
