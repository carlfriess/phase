load("@io_bazel_rules_go//go:def.bzl", "go_binary")

go_binary(
    name = "embed",
    srcs = ["embed.go"],
    visibility = ["//visibility:public"],
)

exports_files(["template.c"])
