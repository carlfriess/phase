package(default_visibility = ["//visibility:public"])

filegroup(
    name = "srcs",
    srcs = [
        "modules/nrfx/mdk/gcc_startup_nrf52.S",
        "components/libraries/log/src/nrf_log_frontend.c",
        "components/libraries/log/src/nrf_log_str_formatter.c",
        "components/boards/boards.c",
        "components/libraries/util/app_error.c",
        "components/libraries/util/app_error_handler_gcc.c",
        "components/libraries/util/app_error_weak.c",
        "components/libraries/util/app_util_platform.c",
        "components/libraries/util/nrf_assert.c",
        "components/libraries/atomic/nrf_atomic.c",
        "components/libraries/balloc/nrf_balloc.c",
        "external/fprintf/nrf_fprintf.c",
        "external/fprintf/nrf_fprintf_format.c",
        "components/libraries/memobj/nrf_memobj.c",
        "components/libraries/ringbuf/nrf_ringbuf.c",
        "components/libraries/strerror/nrf_strerror.c",
        "modules/nrfx/soc/nrfx_atomic.c",
        "modules/nrfx/mdk/system_nrf52.c",
    ],
)

cc_library(
    name = "hdrs",
    srcs = [],
    hdrs = glob([
        "components/libraries/**/*.h",
        "modules/nrfx/*.h",
        "components/*.h",
        "components/boards/*.h",
        "components/drivers_nrf/nrf_soc_nosd/*.h",
        "components/toolchain/cmsis/include/*.h",
        "modules/nrfx/**/*.h",
        "modules/nrfx/hal/*.h",
        "modules/nrfx/mdk/*.h",
        "integration/nrfx/**/*.h",
        "external/fprintf/*.h",
    ]),
    # TODO: Find a better way to add board defines
    defines = [
        "BOARD_PCA10040",
        "NRF52832_XXAA",
        "NRF52",
        "NRF52_PAN_74",
    ] + [
        "APP_TIMER_V2",
        "APP_TIMER_V2_RTC1_ENABLED",
        "CONFIG_GPIO_AS_PINRESET",
        "FLOAT_ABI_HARD",
        "__HEAP_SIZE=8192",
        "__STACK_SIZE=8192",
    ],
    includes = glob(include = [
        "components/libraries/*",
    ], exclude_directories = 0) + [
        "components",
        "components/boards",
        "components/drivers_nrf/nrf_soc_nosd",
        "components/libraries/log/src",
        "components/toolchain/cmsis/include",
        "modules/nrfx",
        "modules/nrfx/drivers/include",
        "modules/nrfx/hal",
        "modules/nrfx/mdk",
        "integration/nrfx",
        "external/fprintf",
    ],
    deps = [
        "modules/nrfx/mdk/nrf_common.ld",
    ],
    linkopts = [
        "-O3 -g3",
        "-mthumb -mabi=aapcs",
        "-mcpu=cortex-m4",
        "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
        "-Wl,--gc-sections",
        "--specs=nano.specs",
        "-lc -lnosys -lm",
        "-L $(location modules/nrfx/mdk)",
    ],
    data = ["modules/nrfx/mdk"],
)

