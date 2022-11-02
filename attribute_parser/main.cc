#include <filesystem>
#include <fstream>
#include "parser.hh"
#include <iostream>
#include <assert.h>

int main(int argc, char *argv[]) {
    
    std::filesystem::path input_file{"attribute_parser/resources/sample_input.txt"};
    assert(std::filesystem::exists(input_file));
    std::ifstream in{input_file.c_str()};
    AttributeParser::parseStream(in);
    return 0;
}