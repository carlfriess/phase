def embed_img(name, img):
    native.genrule(
        name = name,
        srcs = [
            "//tools/img:template.c",
            img,
        ],
        outs = [name + ".c"],
        cmd = "./$(location //tools/img:embed) $(OUTS) $(SRCS)",
        tools = ["//tools/img:embed"],
    )
