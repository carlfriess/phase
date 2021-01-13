package(default_visibility = ["//visibility:public"])

load("@//external/nRF5:nRF5.bzl", "nRF5_copts", "nRF5_linkopts")
load("@//tools/img:embed.bzl", "embed_img")

embed_img(
    name = "com.apple.Passbook",
    img = "@com.apple.Passbook_icon//file",
)

embed_img(
    name = "com.apple.mobilecal",
    img = "@com.apple.mobilecal_icon//file",
)

embed_img(
    name = "com.apple.mobilemail",
    img = "@com.apple.mobilemail_icon//file",
)

embed_img(
    name = "com.apple.reminders",
    img = "@com.apple.reminders_icon//file",
)

embed_img(
    name = "com.burbn.instagram",
    img = "@com.burbn.instagram_icon//file",
)

embed_img(
    name = "com.facebook.Facebook",
    img = "@com.facebook.Facebook_icon//file",
)

embed_img(
    name = "com.facebook.Messenger",
    img = "@com.facebook.Messenger_icon//file",
)

embed_img(
    name = "com.toyopagroup.picaboo",
    img = "@com.toyopagroup.picaboo_icon//file",
)

embed_img(
    name = "net.whatsapp.WhatsApp",
    img = "@net.whatsapp.WhatsApp_icon//file",
)

embed_img(
    name = "ph.telegra.Telegraph",
    img = "@ph.telegra.Telegraph_icon//file",
)

cc_library(
    name = "icons",
    srcs = [
        "icons.cpp",
        # ":com.apple.Passbook",
        # ":com.apple.mobilecal",
        # ":com.apple.mobilemail",
        # ":com.apple.reminders",
        # ":com.burbn.instagram",
        # ":com.facebook.Facebook",
        # ":com.facebook.Messenger",
        # ":com.toyopagroup.picaboo",
        ":net.whatsapp.WhatsApp",
        # ":ph.telegra.Telegraph",
    ],
    hdrs = ["icons.h"],
    copts = nRF5_copts,
    includes = ["."],
    linkopts = nRF5_linkopts,
)
