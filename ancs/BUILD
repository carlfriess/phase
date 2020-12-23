load("@//external/nRF5:nRF5.bzl", "nRF5_binary")

nRF5_binary(
    srcs = [
        "main.c",
    ],
    linker_cmd = "config/ble_app_ancs_c_gcc_nrf52.ld",
    sdk_config_prefix = "config",
)
