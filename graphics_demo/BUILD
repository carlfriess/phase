load("@//external/nRF5:nRF5.bzl", "nRF5_binary")
load("@//tools/img:embed.bzl", "embed_img")

# Photo by Manuel Will on Unsplash
# https://unsplash.com/photos/gd3t5Dtbwkw
embed_img(
    name = "image",
    img = "image.jpeg",
)

nRF5_binary(
    srcs = glob([
        "*.c",
        "*.h",
    ]) + [
        ":image",
        "//watch:spi.c",
        "//watch:spi.h",
        "@nRF5//:components/boards/boards.c",
        "@nRF5//:components/libraries/atomic/nrf_atomic.c",
        "@nRF5//:components/libraries/atomic_fifo/nrf_atfifo.c",
        "@nRF5//:components/libraries/balloc/nrf_balloc.c",
        "@nRF5//:components/libraries/bsp/bsp.c",
        "@nRF5//:components/libraries/button/app_button.c",
        "@nRF5//:components/libraries/log/src/nrf_log_backend_rtt.c",
        "@nRF5//:components/libraries/log/src/nrf_log_backend_serial.c",
        "@nRF5//:components/libraries/log/src/nrf_log_backend_uart.c",
        "@nRF5//:components/libraries/log/src/nrf_log_default_backends.c",
        "@nRF5//:components/libraries/log/src/nrf_log_frontend.c",
        "@nRF5//:components/libraries/log/src/nrf_log_str_formatter.c",
        "@nRF5//:components/libraries/memobj/nrf_memobj.c",
        "@nRF5//:components/libraries/ringbuf/nrf_ringbuf.c",
        "@nRF5//:components/libraries/scheduler/app_scheduler.c",
        "@nRF5//:components/libraries/sortlist/nrf_sortlist.c",
        "@nRF5//:components/libraries/strerror/nrf_strerror.c",
        "@nRF5//:components/libraries/timer/app_timer2.c",
        "@nRF5//:components/libraries/timer/drv_rtc.c",
        "@nRF5//:components/libraries/util/app_error.c",
        "@nRF5//:components/libraries/util/app_error_handler_gcc.c",
        "@nRF5//:components/libraries/util/app_error_weak.c",
        "@nRF5//:components/libraries/util/app_util_platform.c",
        "@nRF5//:components/libraries/util/nrf_assert.c",
        "@nRF5//:external/fprintf/nrf_fprintf.c",
        "@nRF5//:external/fprintf/nrf_fprintf_format.c",
        "@nRF5//:external/segger_rtt/SEGGER_RTT.c",
        "@nRF5//:external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c",
        "@nRF5//:external/segger_rtt/SEGGER_RTT_printf.c",
        "@nRF5//:integration/nrfx/legacy/nrf_drv_uart.c",
        "@nRF5//:modules/nrfx/drivers/src/nrfx_gpiote.c",
        "@nRF5//:modules/nrfx/drivers/src/nrfx_spi.c",
        "@nRF5//:modules/nrfx/drivers/src/nrfx_spim.c",
        "@nRF5//:modules/nrfx/drivers/src/nrfx_ppi.c",
        "@nRF5//:modules/nrfx/drivers/src/nrfx_timer.c",
        "@nRF5//:modules/nrfx/drivers/src/nrfx_uart.c",
        "@nRF5//:modules/nrfx/drivers/src/nrfx_uarte.c",
        "@nRF5//:modules/nrfx/drivers/src/prs/nrfx_prs.c",
        "@nRF5//:modules/nrfx/mdk/gcc_startup_nrf52.S",
        "@nRF5//:modules/nrfx/mdk/system_nrf52.c",
        "@nRF5//:modules/nrfx/soc/nrfx_atomic.c",
    ],
    linker_cmd = "config/blinky_gcc_nrf52.ld",
    sdk_config_prefix = "config",
    deps = [
        "//lib:GC9A01",
    ],
)
