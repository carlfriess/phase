load("@io_bazel_rules_go//go:def.bzl", "go_binary")

go_binary(
    name = "embed",
    srcs = ["embed.go"],
    visibility = ["//visibility:public"],
    deps = ["@com_github_marcinbor85_gohex//:go_default_library"],
)

exports_files(["template.c"])
