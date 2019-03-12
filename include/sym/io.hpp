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

class Tagged {
 protected:
    Tagged(IOTag tag_, const std::string &symbol_) : tag(tag_), symbol(symbol_) {}

 protected:
    IOTag tag;
    std::string symbol;
};

class Input : public Tagged {
 public:
    Input(IOTag tag_, const std::string &symbol_, int num_variables) : Tagged(tag_, symbol_) {
        for (int i = 0; i < num_variables; i++) {
            v.push_back(make_symbol<Variable>(symbol + "[" + std::to_string(i) + "]"));
        }
        Factory::addInput(tag_, symbol, v);
    }
    size_t size() const { return v.size(); }
    const Symbol *data() const { return v.data(); }
    Symbol operator ()(int index) const { return v[index]; }
    void assign(const std::vector<double> &values) {
        for (size_t i = 0; i < values.size(); i++) {
            auto vi = std::dynamic_pointer_cast<Variable>(v[i]);
            vi->assign(values[i]);
        }
    }
    // Function::shared operator [](int index) const { return v[index]; }

 protected:
    std::vector<Symbol> v;
};

class Output : public Tagged {
 public:
    Output(IOTag tag_, const std::string &symbol_, int num_variables) : Tagged(tag_, symbol_), v(num_variables) {
        Factory::addOutput(tag_, symbol, &v);
    }

    size_t size() const { return v.size(); }
    Symbol *data() { return v.data(); }
    const Symbol *data() const { return v.data(); }
    Symbol &operator ()(int index) { return v[index]; }
    const Symbol &operator ()(int index) const { return v[index]; }

 protected:
    std::vector<Symbol> v;
};


class StaticInput : public Input {
 public:
    StaticInput(const std::string &symbol_, int num_variables) : Input(IOTag::STATIC, symbol_, num_variables) {}
};

class DynamicInput : public Input {
 public:
    DynamicInput(const std::string &symbol_, int num_variables) : Input(IOTag::DYNAMIC, symbol_, num_variables) {}
};

class StaticOutput : public Output {
 public:
    StaticOutput(const std::string &symbol_, int num_variables) : Output(IOTag::STATIC, symbol_, num_variables) {}
};

class DynamicOutput : public Output {
 public:
    DynamicOutput(const std::string &symbol_, int num_variables) : Output(IOTag::DYNAMIC, symbol_, num_variables) {}
};

}  // namespace sym

#endif  // IO_HPP_
