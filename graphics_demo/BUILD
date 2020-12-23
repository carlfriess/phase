load("@//external/nRF5:nRF5.bzl", "nRF5_binary")

nRF5_binary(
    srcs = [
        "GC9A01.c",
        "GC9A01.h",
        "main.c",
    ],
    linker_cmd = "config/spi_gcc_nrf52.ld",
    sdk_config_prefix = "config",
)
