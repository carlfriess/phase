load("@bazel_skylib//lib:paths.bzl", "paths")

nRF5_copts = [
    "-O3",
    "-g3",
    "-Wall",
    "-Werror",
    "-ffunction-sections -fdata-sections -fno-strict-aliasing",
    "-fno-builtin -fshort-enums",
] + select({
    "@nRF5//:target_arm_none": [
        "-mcpu=cortex-m4",
        "-mthumb -mabi=aapcs",
        "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
    ],
    "//conditions:default": [],
})

nRF5_linkopts = [
    "-O3 -g3",
] + select({
    "@nRF5//:target_arm_none": [
        "-mthumb -mabi=aapcs",
        "-mcpu=cortex-m4",
        "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
        "-Wl,--gc-sections",
        "-lc -lnosys -lm",
    ],
    "//conditions:default": [],
})

def nRF5_binary(
        name = None,
        srcs = [],
        sdk_config_prefix = None,
        copts = [],
        defines = [],
        includes = [],
        linker_cmd = None,
        linkopts = [],
        board = "PCA10040",
        softdevice = None,
        deps = [],
        **kwargs):
    """Compiles an executable binary using the Nordic nRF5 SDK.

    Args:
        name: A unique name for this target.
        srcs: cc_binary srcs.
        sdk_config_prefix: The prefix to add to the path of the sdk_config.h  file.
        copts: cc_binary copts.
        defines: cc_binary defines.
        includes: cc_binary includes.
        linker: Path or label to an .ld linker command file.
        linkopts: cc_binary linkopts.
        boards: Identifier of target board.
        softdevice: Soft device to build against.
        deps: cc_binary deps.
        **kwargs: Additional cc_binary arguments.
    """

    if name == None:
        name = native.package_name().split("/")[-1]

    if sdk_config_prefix == None:
        fail("SDK configuration prefix not defined for {}: sdk_config_prefix = \"path/to/config\", ".format(name))

    if linker_cmd == None:
        fail("Linker script not defined for {}: linker = \"path/to/linker.ld\", ".format(name))

    if board == "PCA10040":
        defines_board = [
            "BOARD_PCA10040",
            "NRF52832_XXAA",
            "NRF52",
            "NRF52_PAN_74",
        ]
    elif board == "PCA10056":
        defines_board = [
            "BOARD_PCA10056",
            "NRF52840_XXAA",
        ]
    else:
        fail("Unknown board defined for {}: {}".format(name, board))

    if softdevice == None:
        defines_softdevice = []
        deps_softdevice = [
            "@nRF5//:nosd",
        ]
    elif softdevice == "S132":
        defines_softdevice = [
            "NRF_SD_BLE_API_VERSION=7",
            "S132",
            "SOFTDEVICE_PRESENT",
        ]
        deps_softdevice = [
            "@nRF5//:s132",
        ]
    elif softdevice == "S140":
        defines_softdevice = [
            "NRF_SD_BLE_API_VERSION=7",
            "S140",
            "SOFTDEVICE_PRESENT",
        ]
        deps_softdevice = [
            "@nRF5//:s140",
        ]
    else:
        fail("Unknown soft device defined for {}: {}".format(name, softdevice))

    native.cc_binary(
        name = name + "_out",
        srcs = srcs + [
            paths.join(sdk_config_prefix, "sdk_config.h"),
        ],
        copts = copts + nRF5_copts,
        defines = defines + defines_board + defines_softdevice + [
            "APP_TIMER_V2",
            "APP_TIMER_V2_RTC1_ENABLED",
            "CONFIG_GPIO_AS_PINRESET",
            "FLOAT_ABI_HARD",
            "__HEAP_SIZE=8192",
            "__STACK_SIZE=8192",
        ],
        includes = includes + [
            sdk_config_prefix,
        ],
        linkopts = linkopts + nRF5_linkopts + [
            "--specs=nano.specs",
            "-T$(location {})".format(linker_cmd),
        ],
        deps = deps + [
            "@nRF5//:sdk",
            linker_cmd,
        ] + deps_softdevice,
        **kwargs
    )

    native.genrule(
        name = name + "_bin",
        srcs = [":" + name + "_out"],
        outs = [name + ".bin"],
        cmd = "$(execpath @arm_none_eabi//:objcopy) -O binary $< $@",
        tools = ["@arm_none_eabi//:objcopy"],
    )

    native.genrule(
        name = name + "_hex",
        srcs = [":" + name + "_out"],
        outs = [name + ".hex"],
        cmd = "$(execpath @arm_none_eabi//:objcopy) -O ihex $< $@",
        tools = ["@arm_none_eabi//:objcopy"],
    )

    native.alias(
        name = name + "_mergehex",
        actual = select({
            "@nRF5//:host_macOS": "@nRF_tools_macOS//:mergehex",
            "@nRF5//:host_linux": "@nRF_tools_linux//:mergehex",
        }),
        visibility = ["//visibility:private"],
    )

    if softdevice != None:
        native.genrule(
            name = name + "_" + softdevice.lower() + "_hex",
            srcs = [
                ":" + name + "_hex",
                "@nRF5//:components/softdevice/" + softdevice.lower() + "/hex/" + softdevice.lower() + "_nrf52_7.2.0_softdevice.hex",
            ],
            outs = [name + "_" + softdevice.lower() + ".hex"],
            cmd = "$(location :" + name + "_mergehex) -q -m $(SRCS) -o $(OUTS)",
            tools = [":" + name + "_mergehex"],
        )

    native.filegroup(
        name = name,
        srcs = [
            name + "_out",
            name + "_bin",
            name + "_hex",
        ] + [] if softdevice == None else [name + "_" + softdevice.lower() + "_hex"],
    )
