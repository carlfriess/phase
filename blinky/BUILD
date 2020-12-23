load("@//external/nRF5:nRF5.bzl", "nRF5_binary")

nRF5_binary(
    srcs = [
        "main.c",
    ],
    linker_cmd = "config/blinky_gcc_nrf52.ld",
    sdk_config_prefix = "config",
)
