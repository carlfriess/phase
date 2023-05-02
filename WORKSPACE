workspace(name = "phase")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# Library of useful Starlark functions

http_archive(
    name = "bazel_skylib",
    sha256 = "1c531376ac7e5a180e0237938a2536de0c54d93f5c278634818e0efc952dd56c",
    urls = [
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.3/bazel-skylib-1.0.3.tar.gz",
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.3/bazel-skylib-1.0.3.tar.gz",
    ],
)

# Toolchain: GCC ARM Embedded 9.2019q4.major

git_repository(
    name = "arm_none_eabi",
    commit = "7e7d22cc8f166d1f8cd8fcfee0e75742fab26cc7",
    remote = "https://github.com/carlfriess/bazel-arm-none-eabi",
)

load("@arm_none_eabi//:deps.bzl", "arm_none_eabi_deps")

arm_none_eabi_deps()

# Toolchain: Go

http_archive(
    name = "io_bazel_rules_go",
    sha256 = "7904dbecbaffd068651916dce77ff3437679f9d20e1a7956bff43826e7645fcc",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.25.1/rules_go-v0.25.1.tar.gz",
        "https://github.com/bazelbuild/rules_go/releases/download/v0.25.1/rules_go-v0.25.1.tar.gz",
    ],
)

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")

go_rules_dependencies()

go_register_toolchains(version = "1.15.6")

# GoogleTest Framework

git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    tag = "v1.13.0",
)

# Nordic nRF5 SDK

http_archive(
    name = "nRF5",
    build_file = "@//external/nRF5:nRF5.BUILD",
    patches = ["@//external/nRF5:nrf_ble_ancs_c.patch"],
    sha256 = "a577dab8946077d7fcde0e17dcb5460056c371eb599c8d25cbb50b6e9b27096e",
    strip_prefix = "nRF5_SDK_17.0.2_d674dde",
    url = "https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v17.x.x/nRF5_SDK_17.0.2_d674dde.zip",
)

# Nordic nRF command line tools

http_archive(
    name = "nRF_tools_macOS",
    build_file = "@//external/nRF_tools:nRF_tools.BUILD",
    patch_cmds = ["tar -xvf nRF-Command-Line-Tools_10_12_1.tar"],
    sha256 = "16b4f40beee75ec8d7a6b45b5c1b831a7602dc397791a7439282290d37b2a357",
    url = "https://www.nordicsemi.com/-/media/Software-and-other-downloads/Desktop-software/nRF-command-line-tools/sw/Versions-10-x-x/10-12-1/nRF-Command-Line-Tools_10_12_1_OSX.tar",
)

http_archive(
    name = "nRF_tools_linux",
    build_file = "@//external/nRF_tools:nRF_tools.BUILD",
    patch_cmds = ["tar -xvf nRF-Command-Line-Tools_10_12_1.tar"],
    sha256 = "b99558dd631db7f06fe7cfab0a876d352851bbc1e97015432486588b9caf8184",
    type = "tar.gz",
    url = "https://www.nordicsemi.com/-/media/Software-and-other-downloads/Desktop-software/nRF-command-line-tools/sw/Versions-10-x-x/10-12-1/nRFCommandLineTools10121Linuxamd64.tar.gz",
)

# fontem - Embedded Font Library

http_archive(
    name = "fontem",
    build_file = "@//external/fontem:fontem.BUILD",
    patch_cmds = ["touch src/resource/config.h"],
    sha256 = "9e84d52d7c4c252a5110360f5953629839889604074888ee4b8d9ee4758c7c1f",
    strip_prefix = "fontem-475c24b9df89e3def8314b5d89de13c3d3069d25",
    url = "https://github.com/chrisy/fontem/archive/475c24b9df89e3def8314b5d89de13c3d3069d25.zip",
)

# BMI270-Sensor-API

http_archive(
    name = "bmi270",
    build_file = "@//external/bmi270:bmi270.BUILD",
    sha256 = "23e5a6978a44aae8b8df476eec924ce6dd6d003669958473fe878c60b5f14229",
    strip_prefix = "BMI270-Sensor-API-2.71.8",
    url = "https://github.com/BoschSensortec/BMI270-Sensor-API/archive/refs/tags/v2.71.8.zip",
)

# Icon library dependencies

load("//lib/icons:deps.bzl", "icons_deps")

icons_deps()
