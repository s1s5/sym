/**
 * Copyright
 * @file unary_function.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 15:03:25
 */
#ifndef UNARY_FUNCTION_HPP_
#define UNARY_FUNCTION_HPP_

#include <cmath>

#include "function.hpp"
#include "binary_function.hpp"

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
        if (is_zero(arg)) {
            FactoryBase::setAliasRepr(id(), arg->id());
        } else if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(- arg->eval());
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return -arg->eval(); }
};

class SinFunction : public UnaryFunction {
 public:
    SinFunction(const Function::shared &arg_) : UnaryFunction("sin", arg_) {}
    virtual shared diff(shared v) const override;
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::sin(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::sin(arg->eval()); }
};

class CosFunction : public UnaryFunction {
 public:
    CosFunction(const Function::shared &arg_) : UnaryFunction("cos", arg_) {}
    virtual shared diff(shared v) const override;
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::cos(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::cos(arg->eval()); }
};

class SquareRootFunction : public UnaryFunction {
 public:
    SquareRootFunction(const Function::shared &arg_) : UnaryFunction("sqrt", arg_) {}
    virtual shared diff(shared v) const override {
        return std::make_shared<MulFunction>(
            std::make_shared<Constant>(0.5),
            std::make_shared<DivFunction>(arg->diff(v), arg));
    }
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::sqrt(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::sqrt(arg->eval()); }
};

class ExpFunction : public UnaryFunction {
 public:
    ExpFunction(const Function::shared &arg_) : UnaryFunction("exp", arg_) {}
    virtual shared diff(shared v) const override {
        return std::make_shared<MulFunction>(arg->diff(v), arg);
    }
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::exp(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::asin(arg->eval()); }
};

class LogFunction : public UnaryFunction {
 public:
    LogFunction(const Function::shared &arg_) : UnaryFunction("log", arg_) {}
    virtual shared diff(shared v) const override {
        return std::make_shared<DivFunction>(arg->diff(v), arg);
    }
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::log(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::asin(arg->eval()); }
};

class ArcSinFunction : public UnaryFunction {
 public:
    ArcSinFunction(const Function::shared &arg_) : UnaryFunction("asin", arg_) {}
    virtual shared diff(shared v) const override {
        return std::make_shared<DivFunction>(
            arg->diff(v),
            std::make_shared<SquareRootFunction>(
                std::make_shared<SubFunction>(
                    std::make_shared<Constant>(1),
                    std::make_shared<MulFunction>(arg, arg))));
    }
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::asin(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::asin(arg->eval()); }
};

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
