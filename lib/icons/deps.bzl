load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

def icons_deps():
    # Wallet Icon

    http_file(
        name = "com.apple.Passbook_icon",
        sha256 = "767a8df2bd805419ca51933b9f81a5fe0b015b862ccaa2064d6ad08bc5bf15a3",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/2f/e2/70/2fe270d3-c093-57ac-9ed1-50a176d309b1/source/60x60bb.png"],
    )

    # Calendar Icon

    http_file(
        name = "com.apple.mobilecal_icon",
        sha256 = "3fbb49593d54070293f43a876f2235709da5cef85a800cc941a3744b9055f00b",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/50/04/db/5004db8e-ec19-4b1d-d5c4-058d64b1f218/source/60x60bb.png"],
    )

    # Mail Icon

    http_file(
        name = "com.apple.mobilemail_icon",
        sha256 = "4d6f45d08879ac227e56e4fa9ec702a426b5dfc32b7997234b3e1b3e8f0aaf9d",
        urls = ["https://is2-ssl.mzstatic.com/image/thumb/Purple113/v4/37/78/9f/37789f46-04cb-cbb9-e000-08eccbd7b1bf/source/60x60bb.png"],
    )

    # Reminders Icon

    http_file(
        name = "com.apple.reminders_icon",
        sha256 = "faa4a48fcc343b7d833e428de3b778377a2e6e0de6d4a902a583d7323dd6d13c",
        urls = ["https://is4-ssl.mzstatic.com/image/thumb/Purple114/v4/45/73/72/457372a5-8e96-0d97-3f39-f745017b759e/source/60x60bb.png"],
    )

    # Instagram Icon

    http_file(
        name = "com.burbn.instagram_icon",
        sha256 = "8fef8c5f0ab4d5285f1599ca9659fc6498c854281182a1bba3db257eb4ebad80",
        urls = ["https://is3-ssl.mzstatic.com/image/thumb/Purple124/v4/a5/93/44/a593441c-06b4-5f49-98a4-be7824fdcc6a/source/60x60bb.png"],
    )

    # Facebook Icon

    http_file(
        name = "com.facebook.Facebook_icon",
        sha256 = "aef7c8292d08759aa1ca6fdbab996f6edfe28d4184aad7c12a2128465a55a799",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/21/27/fc/2127fca0-66e8-1935-fc29-f917a687905c/source/60x60bb.png"],
    )

    # Messenger Icon

    http_file(
        name = "com.facebook.Messenger_icon",
        sha256 = "4fc5ca4c9965a3e296e78ec67e3363db3bfccb9730980c93344299b4282a9675",
        urls = ["https://is1-ssl.mzstatic.com/image/thumb/Purple124/v4/9c/68/e1/9c68e128-f067-0a97-cc20-125038fd29e5/source/60x60bb.png"],
    )

    # Snapchat Icon

    http_file(
        name = "com.toyopagroup.picaboo_icon",
        sha256 = "3a8cf9da90e918643b2fc7176cf16de742c712ec1860ef190bb650683561cde0",
        urls = ["https://is4-ssl.mzstatic.com/image/thumb/Purple114/v4/ed/6d/b0/ed6db08c-b459-e02b-237f-9decb1e637f7/source/60x60bb.png"],
    )

    # WhatsApp Icon

    http_file(
        name = "net.whatsapp.WhatsApp_icon",
        sha256 = "575d0cdd3d1391c6142189c2e1c0392e09a0034510f05b69c5768fb97c752c42",
        urls = ["https://is5-ssl.mzstatic.com/image/thumb/Purple124/v4/7a/7e/75/7a7e7562-fd84-d0a2-c0af-6a55f0864c0d/source/60x60bb.png"],
    )

    # Teleram Icon

    http_file(
        name = "ph.telegra.Telegraph_icon",
        sha256 = "fbf8ef5e160230864adcc3f62816e05e00acdfb6fea6541e6066b6acc84c8364",
        urls = ["https://is5-ssl.mzstatic.com/image/thumb/Purple114/v4/fa/0d/a5/fa0da5b5-ef54-6669-b252-696851ffca7c/source/60x60bb.png"],
    )
