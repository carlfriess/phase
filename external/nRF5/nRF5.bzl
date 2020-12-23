copts = [
    "-O3",
    "-g3",
    "-mcpu=cortex-m4",
    "-mthumb -mabi=aapcs",
    "-Wall -Werror",
    "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
    "-ffunction-sections -fdata-sections -fno-strict-aliasing",
    "-fno-builtin -fshort-enums",
]