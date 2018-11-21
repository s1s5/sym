/**
 * Copyright 
 * @file factory.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:34:15
 */
#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include "factory_base.hpp"
#include "function.hpp"

namespace sym {

class Factory : public FactoryBase {
 public:
    static void addInput(const std::string &symbol, const std::vector<std::shared_ptr<Variable>> &inputs_) {
        get()->inputs.emplace_back(symbol, inputs_);
    }

    static void addOutput(const std::string &symbol, std::vector<Function::shared> *outputs_) {
        get()->outputs.emplace_back(symbol, outputs_);
    }

 public:
    virtual ~Factory() {}
    Function::shared diff(const Function::shared &func, const Function::shared &var) { return func->diff(var); }

 protected:
    static Factory *get() {
        return dynamic_cast<Factory*>(FactoryBase::get());
    }

 protected:
    std::vector<std::tuple<std::string, std::vector<std::shared_ptr<Variable>>>> inputs;
    std::vector<std::tuple<std::string, std::vector<Function::shared>*>> outputs;
    
};

} // namespace sym

#endif  // FACTORY_HPP_
