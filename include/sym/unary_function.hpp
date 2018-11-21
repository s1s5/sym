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
#include <cmath>

namespace sym {

class UnaryFunction : public Function {
 public:
    UnaryFunction(const std::string &operator_, const Function::shared &arg_)
        : Function(_repr("(", operator_, "(", arg_->id(), "))"), FactoryBase::depends(arg_->id())), arg(arg_) {}

 protected:
    Function::shared arg;
};

class NegFunction : public UnaryFunction {
 public:
    NegFunction(const Function::shared &arg_) : UnaryFunction("-", arg_) {}

    virtual shared diff(shared v) const override {
        if (not FactoryBase::checkDepends(id(), v->id())) { return std::make_shared<Constant>(0); }
        return std::make_shared<NegFunction>(arg->diff(v));
    }
    virtual void simplified() const override {
        arg->simplified();
        if (is_zero(arg)) { FactoryBase::setAliasRepr(id(), arg->id()); }
    }
    virtual double eval() const override { return -arg->eval(); }
};

class SinFunction : public UnaryFunction {
 public:
    SinFunction(const Function::shared &arg_) : UnaryFunction("sin", arg_) {}
    virtual shared diff(shared v) const override;
    virtual void simplified() const override { arg->simplified(); }
    virtual double eval() const override { return std::sin(arg->eval()); }
};

class CosFunction : public UnaryFunction {
 public:
    CosFunction(const Function::shared &arg_) : UnaryFunction("cos", arg_) {}
    virtual shared diff(shared v) const override;
    virtual void simplified() const override { arg->simplified(); }
    virtual double eval() const override { return std::cos(arg->eval()); }
};

class ArcSinFunction : public UnaryFunction {};

class Atan2Function : public UnaryFunction {};

inline Function::shared SinFunction::diff(shared v) const {
    if (not FactoryBase::checkDepends(id(), v->id())) { return std::make_shared<Constant>(0); }
    return std::make_shared<CosFunction>(arg->diff(v));
}

inline Function::shared CosFunction::diff(shared v) const {
    if (not FactoryBase::checkDepends(id(), v->id())) { return std::make_shared<Constant>(0); }
    return std::make_shared<NegFunction>(std::make_shared<SinFunction>(arg->diff(v)));
}

}  // namespace sym

#endif  // UNARY_FUNCTION_HPP_
