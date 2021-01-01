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

http_archive(
    name = "arm_none_eabi",
    sha256 = "3d3728cbe88b08c12cd2cb89afcff9294bd77be958c78188db77fdc8ab7e7a5d",
    strip_prefix = "bazel-arm-none-eabi-1.1",
    url = "https://github.com/d-asnaghi/bazel-arm-none-eabi/archive/v1.1.tar.gz",
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

# Nordic nRF5 SDK

http_archive(
    name = "nRF5",
    build_file = "@//external/nRF5:nRF5.BUILD",
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
    url = "https://www.nordicsemi.com/-/media/Software-and-other-downloads/Desktop-software/nRF-command-line-tools/sw/Versions-10-x-x/10-12-1/nRFCommandLineTools10121Linuxamd64tar.gz",
)

# GoogleTest Framework

git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.10.0",
)
