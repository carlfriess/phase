package(default_visibility = ["//visibility:public"])

filegroup(
    name = "srcs",
    srcs = [
        "components/boards/boards.c",
        "components/libraries/atomic/nrf_atomic.c",
        "components/libraries/balloc/nrf_balloc.c",
        "components/libraries/log/src/nrf_log_frontend.c",
        "components/libraries/log/src/nrf_log_str_formatter.c",
        "components/libraries/memobj/nrf_memobj.c",
        "components/libraries/ringbuf/nrf_ringbuf.c",
        "components/libraries/strerror/nrf_strerror.c",
        "components/libraries/util/app_error.c",
        "components/libraries/util/app_error_handler_gcc.c",
        "components/libraries/util/app_error_weak.c",
        "components/libraries/util/app_util_platform.c",
        "components/libraries/util/nrf_assert.c",
        "external/fprintf/nrf_fprintf.c",
        "external/fprintf/nrf_fprintf_format.c",
        "modules/nrfx/mdk/gcc_startup_nrf52.S",
        "modules/nrfx/mdk/system_nrf52.c",
        "modules/nrfx/soc/nrfx_atomic.c",
    ],
)

filegroup(
    name = "hdrs",
    srcs = glob([
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
    "modules/nrfx",
    "modules/nrfx/drivers/include",
    "modules/nrfx/hal",
    "modules/nrfx/mdk",
    "integration/nrfx",
    "external/fprintf",
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
