/**
 * Copyright 
 * @file unary_function.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 15:03:25
 */
#ifndef UNARY_FUNCTION_HPP_
#define UNARY_FUNCTION_HPP_

#include "function.hpp"

namespace sym {

class NegFunction : public Function {
 public:
    NegFunction(const Function::shared &arg_) : Function("(-" + arg_->repr() + ")", FactoryBase::depends(arg_->id())), arg(arg_) {}

    virtual shared diff(shared v) const override {
        return std::make_shared<NegFunction>(arg->diff(v));
    }
    virtual void simplified() const override {
        arg->simplified();
        if (is_zero(arg)) {
            FactoryBase::setAliasRepr(id(), arg->id());
        }
    }
    virtual double eval() const override { return - arg->eval(); }
    
 protected:
    Function::shared arg;
};

}  // namespace sym

#endif  // UNARY_FUNCTION_HPP_
