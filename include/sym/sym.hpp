/**
 * Copyright
 * @file sym.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 12:29:40
 */
#ifndef SYM_HPP_
#define SYM_HPP_

#include "factory_base.hpp"
#include "function.hpp"
#include "unary_function.hpp"
#include "binary_function.hpp"
#include "calculation_graph.hpp"
#include "cxx_code_printer.hpp"
#include "factory.hpp"
#include "io.hpp"
#include "unary_function_impl.hpp"
#include "binary_function_impl.hpp"

#include <iostream>

namespace sym {

template<class F>
int default_main(int argc, char **argv) {
    F factory;

    bool test_mode = false, dag_mode = false;
    std::string function_name, ns_name, class_name;
    for (int arg = 1; arg < argc; arg++) {
        auto check_arg = [&](char short_opt, const std::string &long_opt, bool increment_arg) {
            if (std::string(argv[arg]) == std::string("-") + std::string({short_opt}) or
                std::string(argv[arg]) == std::string("--") + long_opt) {
                if (increment_arg) {
                    arg++;
                }
                return true;
            }
            return false;
        };
        if (check_arg('f', "function-name", true)) {
            function_name = argv[arg];
        } else if (check_arg('n', "namespace", true)) {
            ns_name = argv[arg];
        } else if (check_arg('c', "class", true)) {
            class_name = argv[arg];
        } else if (check_arg('\0', "test", false)) {
            test_mode = true;
        } else if (check_arg('\0', "dag", false)) {
            dag_mode = true;
        }
    }

    factory.generate();

    if (test_mode) {
        
    } else if (dag_mode) {
        std::cout << factory.digraph() << std::endl;
    } else if (class_name != "") {
        std::cout << factory.cxxCodePrinter(ns_name, class_name) << std::endl;
    } else if (function_name != "") {
        
    } else {
        std::cerr << "no option specified" << std::endl;
        return -1;
    }
    return 0;
}

}  // namespace


#define OUTPUT_CXX_CODE_MAIN(FACTORY_NAME)                              \
    int main(int argc, char **argv) {                                   \
        return ::sym::default_main<FACTORY_NAME>(argc, argv);           \
    }

#endif  // SYM_HPP_
