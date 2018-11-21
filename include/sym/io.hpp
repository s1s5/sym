/**
 * Copyright 
 * @file io.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:41:57
 */
#ifndef IO_HPP_
#define IO_HPP_

#include "function.hpp"
#include "factory.hpp"

namespace sym {

class Input {
    Input(const std::string &symbol, int num_variables) {
        for (int i = 0; i < num_variables; i++) {
            v.push_back(std::make_shared<Variable>(symbol + "[" + std::to_string(i) + "]"));
        }
    }
    
    const std::shared_ptr<Function> &operator [](int index) const { return v[index]; }

 protected:
    std::vector<std::shared_ptr<Function>> v;
};

class Output {
 protected:
    Output(const std::string &symbol_, int num_variables) : symbol(symbol_), v(num_variables, nullptr) {
    }

 protected:
    std::string symbol;
    std::vector<std::shared_ptr<Function>> v;
};

}  // namespace sym

#endif  // IO_HPP_
