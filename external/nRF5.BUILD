package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sdk",
    # TODO: Find a better way to add soft devices
    srcs = glob(include=[
        "components/softdevice/common/*.h",
        "components/softdevice/s140/headers/*.h",
    ]) + glob(include = [
        "components/libraries/**/*.c",
        "components/boards/*.c",
        "external/fprintf/*.c",
    ], exclude = [
        "components/libraries/bootloader/**/*"
    ]) + [
        "modules/nrfx/soc/nrfx_atomic.c",
    ],
    # TODO: Find a better way to add soft devices
    hdrs = glob(include=[
        "components/softdevice/common/*.h",
        "components/softdevice/s140/headers/*.h",
    ]) + glob([
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
    # TODO: Find a better way to add SDK config
    ]) + [
        "examples/peripheral/blinky/pca10040/blank/config/sdk_config.h",
    ],
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
    # TODO: Find a better way to add soft devices
    includes = [
        "components/softdevice/common",
        "components/softdevice/s140/headers",
    ] + glob(include=[
        "components/libraries/*",
    ], exclude_directories=0) + [
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
        # TODO: Find a better way to add SDK config
        "examples/peripheral/blinky/pca10040/blank/config",
    ],
    copts = [
        "-O3",
        "-g3",
        "-mcpu=cortex-m4",
        "-mthumb -mabi=aapcs",
        "-Wall -Werror",
        "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
        "-ffunction-sections -fdata-sections -fno-strict-aliasing",
        "-fno-builtin -fshort-enums",
    ],
    linkopts = [
        "-O3 -g3",
        "-mthumb -mabi=aapcs",
        "-mcpu=cortex-m4",
        "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
        "-Wl,--gc-sections",
        "--specs=nano.specs",
        "-lc -lnosys -lm",
    ],
    deps = [],
)


#  $(PROJ_DIR) \
#  ../config \
