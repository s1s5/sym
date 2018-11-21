/**
 * Copyright 
 * @file binary_function.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:41:13
 */
#ifndef BINARY_FUNCTION_HPP_
#define BINARY_FUNCTION_HPP_

#include "function.hpp"

namespace sym {

class BinaryFunction : public Function {
 public:
    BinaryFunction(const std::string &operator_, const Function::shared &arg0_, const Function::shared &arg1_) :
        Function("(" + arg0_->repr() + operator_ + arg1_->repr() + ")", FactoryBase::depends(arg0_->id(), arg1_->id())),
        arg0(arg0_), arg1(arg1_) {}
 protected:
    Function::shared arg0, arg1;
};

class AddFunction : public BinaryFunction {
 public:
    AddFunction(const Function::shared &arg0, const Function::shared &arg1) : BinaryFunction("+", arg0, arg1) {}
    virtual shared diff(shared v) const override {
        return std::make_shared<AddFunction>(arg0->diff(v), arg1->diff(v));
    }
    virtual void simplified() const override {
        arg0->simplified();
        arg1->simplified();
        if (is_zero(arg0)) {
            FactoryBase::setAliasRepr(id(), arg1->id());
        } else if (is_zero(arg1)) {
            FactoryBase::setAliasRepr(id(), arg0->id());
        }
    }
};

Function::shared operator + (const Function::shared &arg0, const Function::shared &arg1) {
    return std::make_shared<AddFunction>(arg0, arg1);
}

}  // namespace sym

#endif  // BINARY_FUNCTION_HPP_
