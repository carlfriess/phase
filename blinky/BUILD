load("@//external/nRF5:nRF5.bzl", "copts", "linkopts", "defines")

cc_binary(
    name = "blinky",
    srcs = [
        "@nRF5//:srcs",
        "main.c",
        "config/sdk_config.h",
    ],
    copts = copts,
    linkopts = linkopts + [
        "-T$(location config/blinky_gcc_nrf52.ld)"
    ],
    includes = ["config"],
    defines = defines,
    deps = [
        "@nRF5//:sdk",
        "config/blinky_gcc_nrf52.ld",
    ],
)

genrule(
    name = "blinky_hex",
    outs = ["blinky.hex"],
    srcs = [":blinky"],
    cmd = "$(execpath @arm_none_eabi//:objcopy) -O ihex $< $@",
    tools = ["@arm_none_eabi//:objcopy"],
)