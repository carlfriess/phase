package(default_visibility = ["//visibility:public"])

filegroup(
    name = "srcs",
    srcs = [
        "components/boards/boards.c",
        "components/libraries/atomic/nrf_atomic.c",
        "components/libraries/atomic_fifo/nrf_atfifo.c",
        "components/libraries/balloc/nrf_balloc.c",
        "components/libraries/bsp/bsp.c",
        "components/libraries/button/app_button.c",
        "components/libraries/log/src/nrf_log_backend_rtt.c",
        "components/libraries/log/src/nrf_log_backend_serial.c",
        "components/libraries/log/src/nrf_log_backend_uart.c",
        "components/libraries/log/src/nrf_log_default_backends.c",
        "components/libraries/log/src/nrf_log_frontend.c",
        "components/libraries/log/src/nrf_log_str_formatter.c",
        "components/libraries/memobj/nrf_memobj.c",
        "components/libraries/ringbuf/nrf_ringbuf.c",
        "components/libraries/scheduler/app_scheduler.c",
        "components/libraries/sortlist/nrf_sortlist.c",
        "components/libraries/strerror/nrf_strerror.c",
        "components/libraries/timer/app_timer2.c",
        "components/libraries/timer/drv_rtc.c",
        "components/libraries/util/app_error.c",
        "components/libraries/util/app_error_handler_gcc.c",
        "components/libraries/util/app_error_weak.c",
        "components/libraries/util/app_util_platform.c",
        "components/libraries/util/nrf_assert.c",
        "external/fprintf/nrf_fprintf.c",
        "external/fprintf/nrf_fprintf_format.c",
        "external/segger_rtt/SEGGER_RTT.c",
        "external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c",
        "external/segger_rtt/SEGGER_RTT_printf.c",
        "integration/nrfx/legacy/nrf_drv_spi.c",
        "integration/nrfx/legacy/nrf_drv_uart.c",
        "modules/nrfx/drivers/src/nrfx_gpiote.c",
        "modules/nrfx/drivers/src/nrfx_spi.c",
        "modules/nrfx/drivers/src/nrfx_spim.c",
        "modules/nrfx/drivers/src/nrfx_uart.c",
        "modules/nrfx/drivers/src/nrfx_uarte.c",
        "modules/nrfx/drivers/src/prs/nrfx_prs.c",
        "modules/nrfx/mdk/gcc_startup_nrf52.S",
        "modules/nrfx/mdk/system_nrf52.c",
        "modules/nrfx/soc/nrfx_atomic.c",
    ],
)

filegroup(
    name = "hdrs",
    srcs = glob([
        "components/*.h",
        "components/boards/*.h",
        "components/drivers_nrf/nrf_soc_nosd/*.h",
        "components/libraries/**/*.h",
        "components/toolchain/cmsis/include/*.h",
        "external/fprintf/*.h",
        "external/segger_rtt/*.h",
        "integration/nrfx/**/*.h",
        "modules/nrfx/**/*.h",
        "modules/nrfx/*.h",
        "modules/nrfx/hal/*.h",
        "modules/nrfx/mdk/*.h",
    ]),
)

includes = glob(
    include = [
        "components/libraries/*",
    ],
    exclude_directories = 0,
) + [
    "components",
    "components/boards",
    "components/drivers_nrf/nrf_soc_nosd",
    "components/libraries/log/src",
    "components/toolchain/cmsis/include",
    "external/fprintf",
    "external/segger_rtt",
    "integration/nrfx",
    "integration/nrfx/legacy",
    "modules/nrfx",
    "modules/nrfx/drivers/include",
    "modules/nrfx/hal",
    "modules/nrfx/mdk",
]

cc_library(
    name = "sdk",
    srcs = [":hdrs"],
    hdrs = [":hdrs"],
    data = ["modules/nrfx/mdk"],
    includes = includes,
    linkopts = [
        "-L $(location @nRF5//:modules/nrfx/mdk)",
    ],
    linkstatic = 1,
    deps = [
        "modules/nrfx/mdk/nrf_common.ld",
    ],
)

exports_files(glob(["**"]))
