load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")
load("@bazel_gazelle//:deps.bzl", "go_repository")

def icons_deps():
    # gohex package
    go_repository(
        name = "com_github_marcinbor85_gohex",
        importpath = "github.com/marcinbor85/gohex",
        commit = "55fb1c624d845f0f5b79ee946cf09a15cb50ed89",
    )

    # Wallet Icon
    http_file(
        name = "com.apple.Passbook_icon",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/2f/e2/70/2fe270d3-c093-57ac-9ed1-50a176d309b1/source/60x60bb.png"],
    )

    # Calendar Icon
    http_file(
        name = "com.apple.mobilecal_icon",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/50/04/db/5004db8e-ec19-4b1d-d5c4-058d64b1f218/source/60x60bb.png"],
    )

    # Mail Icon
    http_file(
        name = "com.apple.mobilemail_icon",
        urls = ["https://is2-ssl.mzstatic.com/image/thumb/Purple113/v4/37/78/9f/37789f46-04cb-cbb9-e000-08eccbd7b1bf/source/60x60bb.png"],
    )

    # Reminders Icon
    http_file(
        name = "com.apple.reminders_icon",
        urls = ["https://is4-ssl.mzstatic.com/image/thumb/Purple114/v4/45/73/72/457372a5-8e96-0d97-3f39-f745017b759e/source/60x60bb.png"],
    )

    # Instagram Icon
    http_file(
        name = "com.burbn.instagram_icon",
        urls = ["https://is3-ssl.mzstatic.com/image/thumb/Purple124/v4/a5/93/44/a593441c-06b4-5f49-98a4-be7824fdcc6a/source/60x60bb.png"],
    )

    # Facebook Icon
    http_file(
        name = "com.facebook.Facebook_icon",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/21/27/fc/2127fca0-66e8-1935-fc29-f917a687905c/source/60x60bb.png"],
    )

    # Messenger Icon
    http_file(
        name = "com.facebook.Messenger_icon",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/9c/68/e1/9c68e128-f067-0a97-cc20-125038fd29e5/source/60x60bb.png"],
    )

    # Snapchat Icon
    http_file(
        name = "com.toyopagroup.picaboo_icon",
        urls = ["https://is4-ssl.mzstatic.com/image/thumb/Purple114/v4/ed/6d/b0/ed6db08c-b459-e02b-237f-9decb1e637f7/source/60x60bb.png"],
    )

    # WhatsApp Icon
    http_file(
        name = "net.whatsapp.WhatsApp_icon",
        urls = ["https://is5-ssl.mzstatic.com/image/thumb/Purple124/v4/7a/7e/75/7a7e7562-fd84-d0a2-c0af-6a55f0864c0d/source/60x60bb.png"],
    )

    # Teleram Icon
    http_file(
        name = "ph.telegra.Telegraph_icon",
        urls = ["https://is5-ssl.mzstatic.com/image/thumb/Purple114/v4/fa/0d/a5/fa0da5b5-ef54-6669-b252-696851ffca7c/source/60x60bb.png"],
    )

    # Pushover Icon
    http_file(
        name = "net.superblock.Pushover_icon",
        urls = ["https://is2-ssl.mzstatic.com/image/thumb/Purple112/v4/df/08/b1/df08b10d-9437-d4ab-6445-10fd82753bae/AppIcon-0-0-1x_U007emarketing-0-0-0-7-0-0-sRGB-0-0-0-GLES2_U002c0-512MB-85-220-0-0.png/60x60bb.png"],
    )

    # BeReal Icon
    http_file(
        name = "AlexisBarreyat.BeReal_icon",
        urls = ["https://is4-ssl.mzstatic.com/image/thumb/Purple116/v4/dc/ad/60/dcad60f1-6aa3-78b1-32fe-91d20fb29bfb/AppIcon-1x_U007emarketing-0-5-0-85-220.png/60x60bb.jpg"],
    )
