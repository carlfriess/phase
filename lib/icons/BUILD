package(default_visibility = ["//visibility:public"])

load("@//external/nRF5:nRF5.bzl", "nRF5_copts", "nRF5_linkopts")
load("@//tools/img:embed.bzl", "embed_hex")

icons = [
    {
        "name": "com.apple.Passbook",
        "img": "@com.apple.Passbook_icon//file",
    },
    {
        "name": "com.apple.mobilecal",
        "img": "@com.apple.mobilecal_icon//file",
    },
    {
        "name": "com.apple.mobilemail",
        "img": "@com.apple.mobilemail_icon//file",
    },
    {
        "name": "com.apple.reminders",
        "img": "@com.apple.reminders_icon//file",
    },
    {
        "name": "com.burbn.instagram",
        "img": "@com.burbn.instagram_icon//file",
    },
    {
        "name": "com.facebook.Facebook",
        "img": "@com.facebook.Facebook_icon//file",
    },
    {
        "name": "com.facebook.Messenger",
        "img": "@com.facebook.Messenger_icon//file",
    },
    {
        "name": "com.toyopagroup.picaboo",
        "img": "@com.toyopagroup.picaboo_icon//file",
    },
    {
        "name": "net.whatsapp.WhatsApp",
        "img": "@net.whatsapp.WhatsApp_icon//file",
    },
    {
        "name": "ph.telegra.Telegraph",
        "img": "@ph.telegra.Telegraph_icon//file",
    },
    {
        "name": "net.superblock.Pushover",
        "img": "@net.superblock.Pushover_icon//file",
    },
    {
        "name": "AlexisBarreyat.BeReal",
        "img": "@AlexisBarreyat.BeReal_icon//file",
    },
    {
        "name": "com.hammerandchisel.discord",
        "img": "@com.hammerandchisel.discord_icon//file",
    },
    {
        "name": "com.mattermost.rn",
        "img": "@com.mattermost.rn_icon//file",
    },
    {
        "name": "uk.co.bbc.news",
        "img": "@uk.co.bbc.news_icon//file",
    },
    {
        "name": "com.apnews.ipad.mobilenewsdevel",
        "img": "@com.apnews.ipad.mobilenewsdevel_icon//file",
    },
]

embed_hex(
    name = "icons_hex",
    imgs = icons,
    template = "icons.cpp",
)

cc_library(
    name = "icons",
    srcs = [
        ":icons_hex",
    ],
    hdrs = ["icons.h"],
    copts = nRF5_copts,
    includes = ["."],
    linkopts = nRF5_linkopts,
)
