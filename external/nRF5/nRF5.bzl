copts = [
    "-O3",
    "-g3",
    "-mcpu=cortex-m4",
    "-mthumb -mabi=aapcs",
    "-Wall",
    "-Werror",
    "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
    "-ffunction-sections -fdata-sections -fno-strict-aliasing",
    "-fno-builtin -fshort-enums",
]

linkopts = [
    "-O3 -g3",
    "-mthumb -mabi=aapcs",
    "-mcpu=cortex-m4",
    "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
    "-Wl,--gc-sections",
    "--specs=nano.specs",
    "-lc -lnosys -lm",
]

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
]