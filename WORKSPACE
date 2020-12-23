workspace(name = "phase")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

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

# Nordic nRF5 SDK

http_archive(
    name = "nRF5",
    build_file = "@//external/nRF5:BUILD",
    sha256 = "a577dab8946077d7fcde0e17dcb5460056c371eb599c8d25cbb50b6e9b27096e",
    strip_prefix = "nRF5_SDK_17.0.2_d674dde",
    url = "https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v17.x.x/nRF5_SDK_17.0.2_d674dde.zip",
)