def embed_img(name, img):
    native.genrule(
        name = name,
        srcs = [
            "//tools/img:template.c",
            img,
        ],
        outs = [name + ".c"],
        cmd = "./$(location //tools/img:embed) -output $(OUTS) -template $(location //tools/img:template.c) -input $(location {})".format(img),
        tools = ["//tools/img:embed"],
    )
