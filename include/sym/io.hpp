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
 public:
    Input(const std::string &symbol, int num_variables) {
        for (int i = 0; i < num_variables; i++) {
            v.push_back(std::make_shared<Variable>(symbol + "[" + std::to_string(i) + "]"));
        }
        Factory::addInput(symbol, v);
    }
    
    std::shared_ptr<Variable> operator [](int index) const { return v[index]; }
    // Function::shared operator [](int index) const { return v[index]; }

 protected:
    std::vector<std::shared_ptr<Variable>> v;
};

class Output {
 public:
    Output(const std::string &symbol_, int num_variables) : symbol(symbol_), v(num_variables, nullptr) {
        Factory::addOutput(symbol, &v);
    }

    Function::shared &operator [](int index) { return v[index]; }

 protected:
    std::string symbol;
    std::vector<Function::shared> v;
};

}  // namespace sym

#endif  // IO_HPP_
