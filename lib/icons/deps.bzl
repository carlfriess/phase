load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

def icons_deps():
    # Wallet Icon

    http_file(
        name = "com.apple.Passbook_icon",
        sha256 = "5724b0ce6a9da77831968787223790547c6f07786de0c76d955b7f138f221d88",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/2f/e2/70/2fe270d3-c093-57ac-9ed1-50a176d309b1/source/60x60bb.jpg"],
    )

    # Calendar Icon

    http_file(
        name = "com.apple.mobilecal_icon",
        sha256 = "4f09ebc04ad4c7a32e7fdbfb35851ffc8dd593ead04350cab7872e45ec43a3da",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/50/04/db/5004db8e-ec19-4b1d-d5c4-058d64b1f218/source/60x60bb.jpg"],
    )

    # Mail Icon

    http_file(
        name = "com.apple.mobilemail_icon",
        sha256 = "e56fc05113881b4fea5f61951e1f33a95ba2034cfac8f10c1019e637d2fc7c1d",
        urls = ["https://is2-ssl.mzstatic.com/image/thumb/Purple113/v4/37/78/9f/37789f46-04cb-cbb9-e000-08eccbd7b1bf/source/60x60bb.jpg"],
    )

    # Reminders Icon

    http_file(
        name = "com.apple.reminders_icon",
        sha256 = "5fd482a1b398ec0692d9c3cd9a4f4781a745792f39fc2b6e9bf2acca373337b7",
        urls = ["https://is4-ssl.mzstatic.com/image/thumb/Purple114/v4/45/73/72/457372a5-8e96-0d97-3f39-f745017b759e/source/60x60bb.jpg"],
    )

    # Instagram Icon

    http_file(
        name = "com.burbn.instagram_icon",
        sha256 = "5e221b4c13c437954acd613fb63f6286d8cfe5827ff6daa897b28f8889c015a8",
        urls = ["https://is3-ssl.mzstatic.com/image/thumb/Purple124/v4/a5/93/44/a593441c-06b4-5f49-98a4-be7824fdcc6a/source/60x60bb.jpg"],
    )

    # Facebook Icon

    http_file(
        name = "com.facebook.Facebook_icon",
        sha256 = "612bd7c10453a7788a50979b5f9a49128e5e8316cb7f0317e4664cfa003e19ba",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/21/27/fc/2127fca0-66e8-1935-fc29-f917a687905c/source/60x60bb.jpg"],
    )

    # Messenger Icon

    http_file(
        name = "com.facebook.Messenger_icon",
        sha256 = "22d27a39d67fec09bf80ae60c05dc640e31387fd03d3fd8a5644b0b9c12e84bc",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/9c/68/e1/9c68e128-f067-0a97-cc20-125038fd29e5/source/60x60bb.jpg"],
    )

    # Snapchat Icon

    http_file(
        name = "com.toyopagroup.picaboo_icon",
        sha256 = "8c6e5d107cef1c32c06306f2c2366f036318e41ff92496ff827e61f2ac0923fb",
        urls = ["https://is4-ssl.mzstatic.com/image/thumb/Purple114/v4/ed/6d/b0/ed6db08c-b459-e02b-237f-9decb1e637f7/source/60x60bb.jpg"],
    )

    # WhatsApp Icon

    http_file(
        name = "net.whatsapp.WhatsApp_icon",
        sha256 = "2474e65ed2a9764fa66f28046b638de3d14cc1c82483d01b49ade19025d50109",
        urls = ["https://is5-ssl.mzstatic.com/image/thumb/Purple124/v4/7a/7e/75/7a7e7562-fd84-d0a2-c0af-6a55f0864c0d/source/60x60bb.jpg"],
    )

    # Teleram Icon

    http_file(
        name = "ph.telegra.Telegraph_icon",
        sha256 = "e56fc05113881b4fea5f61951e1f33a95ba2034cfac8f10c1019e637d2fc7c1d",
        urls = ["https://is5-ssl.mzstatic.com/image/thumb/Purple114/v4/fa/0d/a5/fa0da5b5-ef54-6669-b252-696851ffca7c/source/60x60bb.jpg"],
    )
