/**
 * Copyright
 * @file cpput_main.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-20 16:55:22
 */
#include "cpput.hpp"

int main(int argc, char **argv) {
    if (argc == 2 && std::string(argv[1]) == "--xml") {
        ::cpput::XmlResultWriter writer;
        return ::cpput::runAllTests(writer);
    }
    ::cpput::TextResultWriter writer;
    return ::cpput::runAllTests(writer);
}

