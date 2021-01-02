package(default_visibility = ["//visibility:public"])

load("@//external/nRF5:nRF5.bzl", "nRF5_copts", "nRF5_linkopts")

cc_library(
    name = "quicksand-64",
    srcs = ["font-quicksand-64.c"],
    hdrs = ["font-quicksand-64.h"],
    copts = nRF5_copts,
    includes = ["."],
    linkopts = nRF5_linkopts,
    deps = ["@fontem//:libfontem"],
)
