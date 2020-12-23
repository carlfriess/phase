load("@//external/nRF5:nRF5.bzl", "copts")

cc_binary(
    name = "blinky",
    srcs = [
        "@nRF5//:srcs",
        "main.c",
        "config/sdk_config.h",
    ],
    copts = copts,
    linkopts = [
        "-T$(location config/blinky_gcc_nrf52.ld)"
    ],
    includes = ["config"],
    deps = [
        "@nRF5//:hdrs",
        "config/blinky_gcc_nrf52.ld",
    ],
)