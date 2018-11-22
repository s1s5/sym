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


namespace sym {

template<class F>
int default_main(int arg, char **argv) {
    F factory;
    return 0;
}

}  // namespace


#define OUTPUT_CXX_CODE_MAIN(FACTORY_NAME)                              \
    int main(int argc, char **argv) {                                   \
        return ::sym::default_main<FACTORY_NAME>(argc, argv);           \
    }

#endif  // SYM_HPP_
