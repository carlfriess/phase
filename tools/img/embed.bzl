def embed_img(name, img):
    native.genrule(
        name = name,
        srcs = [
            "//tools/img:template.c",
            img,
        ],
        outs = [name + ".c"],
        cmd = "./$(location //tools/img:embed) -single -name {} -input $(location {}) -output $(OUTS) -template $(location //tools/img:template.c)".format(name, img),
        tools = ["//tools/img:embed"],
    )

def embed_mask(name, img):
    native.genrule(
        name = name,
        srcs = [
            "//tools/img:template.c",
            img,
        ],
        outs = [name + ".c"],
        cmd = "./$(location //tools/img:embed) -mask -name {} -input $(location {}) -output $(OUTS) -template $(location //tools/img:template.c)".format(name, img),
        tools = ["//tools/img:embed"],
    )

def embed_hex(name, imgs, template):
    cmd = "./$(location //tools/img:embed) -hex"
    for img in imgs:
        cmd += " -name {} -input $(location {})".format(img["name"], img["img"])
    cmd += " -template $(location {}) -output $(location images.hex) -output $(location index.cpp)".format(template)
    native.genrule(
        name = name,
        outs = ["images.hex", "index.cpp"],
        srcs = [i["img"] for i in imgs] + [template],
        cmd = cmd,
        tools = ["//tools/img:embed"],
    )
