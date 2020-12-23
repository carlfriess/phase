load("@//external/nRF5:nRF5.bzl", "copts", "defines", "linkopts")

cc_binary(
    name = "blinky",
    srcs = [
        "config/sdk_config.h",
        "main.c",
        "@nRF5//:srcs",
    ],
    copts = copts,
    defines = defines,
    includes = ["config"],
    linkopts = linkopts + [
        "-T$(location config/blinky_gcc_nrf52.ld)",
    ],
    deps = [
        "config/blinky_gcc_nrf52.ld",
        "@nRF5//:sdk",
    ],
)

genrule(
    name = "blinky_hex",
    srcs = [":blinky"],
    outs = ["blinky.hex"],
    cmd = "$(execpath @arm_none_eabi//:objcopy) -O ihex $< $@",
    tools = ["@arm_none_eabi//:objcopy"],
)
