cc_library(
    name="attribute_parser",
    hdrs = ["parser.hh"],
    srcs = ["parser.cc"],
)

cc_binary(
    name="main",
    srcs = ["main.cc"],
    data=["resources/sample_input.txt"],
    deps=[":attribute_parser"],
)