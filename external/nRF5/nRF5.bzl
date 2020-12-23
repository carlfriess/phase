load("@bazel_skylib//lib:paths.bzl", "paths")

def nRF5_binary(
        name = None,
        srcs = [],
        sdk_config_prefix = None,
        copts = [],
        defines = [],
        includes = [],
        linker_cmd = None,
        linkopts = [],
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
        deps: cc_binary deps.
        **kwargs: Additional cc_binary arguments.
    """

    if name == None:
        name = native.package_name().split("/")[-1]

    if linker_cmd == None:
        fail("Linker script not defined for {}: linker = \"path/to/linker.ld\", ".format(name))

    native.cc_binary(
        name = name + "_out",
        srcs = srcs + [
            paths.join(sdk_config_prefix, "sdk_config.h"),
            "@nRF5//:srcs",
        ],
        copts = copts + [
            "-O3",
            "-g3",
            "-mcpu=cortex-m4",
            "-mthumb -mabi=aapcs",
            "-Wall",
            "-Werror",
            "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
            "-ffunction-sections -fdata-sections -fno-strict-aliasing",
            "-fno-builtin -fshort-enums",
        ],
        # TODO: Find a better way to add board defines
        defines = defines + [
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
        includes = includes + [
            sdk_config_prefix,
        ],
        linkopts = linkopts + [
            "-O3 -g3",
            "-mthumb -mabi=aapcs",
            "-mcpu=cortex-m4",
            "-mfloat-abi=hard -mfpu=fpv4-sp-d16",
            "-Wl,--gc-sections",
            "--specs=nano.specs",
            "-lc -lnosys -lm",
        ] + [
            "-T$(location {})".format(linker_cmd),
        ],
        deps = deps + [
            "@nRF5//:sdk",
            linker_cmd,
        ],
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

    native.filegroup(
        name = name,
        srcs = [
            name + "_out",
            name + "_bin",
            name + "_hex",
        ],
    )