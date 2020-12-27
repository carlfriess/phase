load("@//external/nRF5:nRF5.bzl", "nRF5_binary")

nRF5_binary(
    srcs = [
        "main.c",
        "@nRF5//:components/boards/boards.c",
        "@nRF5//:components/libraries/atomic/nrf_atomic.c",
        "@nRF5//:components/libraries/balloc/nrf_balloc.c",
        "@nRF5//:components/libraries/log/src/nrf_log_frontend.c",
        "@nRF5//:components/libraries/log/src/nrf_log_str_formatter.c",
        "@nRF5//:components/libraries/memobj/nrf_memobj.c",
        "@nRF5//:components/libraries/ringbuf/nrf_ringbuf.c",
        "@nRF5//:components/libraries/strerror/nrf_strerror.c",
        "@nRF5//:components/libraries/util/app_error.c",
        "@nRF5//:components/libraries/util/app_error_handler_gcc.c",
        "@nRF5//:components/libraries/util/app_error_weak.c",
        "@nRF5//:components/libraries/util/app_util_platform.c",
        "@nRF5//:components/libraries/util/nrf_assert.c",
        "@nRF5//:external/fprintf/nrf_fprintf.c",
        "@nRF5//:external/fprintf/nrf_fprintf_format.c",
        "@nRF5//:modules/nrfx/mdk/gcc_startup_nrf52.S",
        "@nRF5//:modules/nrfx/mdk/system_nrf52.c",
        "@nRF5//:modules/nrfx/soc/nrfx_atomic.c",
    ],
    linker_cmd = "config/ble_app_ancs_c_gcc_nrf52.ld",
    sdk_config_prefix = "config",
)
