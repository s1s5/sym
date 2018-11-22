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
 protected:
    virtual shared _diff(shared v) const override {
        return std::make_shared<NegFunction>(arg->diff(v));
    }
};

class SinFunction : public UnaryFunction {
 public:
    SinFunction(const Function::shared &arg_) : UnaryFunction("sin", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::sin(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::sin(arg->eval()); }
 protected:
    virtual shared _diff(shared v) const override;
};

class CosFunction : public UnaryFunction {
 public:
    CosFunction(const Function::shared &arg_) : UnaryFunction("cos", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::cos(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::cos(arg->eval()); }
 protected:
    virtual shared _diff(shared v) const override;
};

class SquareRootFunction : public UnaryFunction {
 public:
    SquareRootFunction(const Function::shared &arg_) : UnaryFunction("sqrt", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::sqrt(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::sqrt(arg->eval()); }
 protected:
    virtual shared _diff(shared v) const override {
        return std::make_shared<MulFunction>(
            std::make_shared<Constant>(0.5),
            std::make_shared<DivFunction>(arg->diff(v), arg));
    }
};

class ExpFunction : public UnaryFunction {
 public:
    ExpFunction(const Function::shared &arg_) : UnaryFunction("exp", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::exp(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::asin(arg->eval()); }
 protected:
    virtual shared _diff(shared v) const override {
        return std::make_shared<MulFunction>(arg->diff(v), arg);
    }
};

class LogFunction : public UnaryFunction {
 public:
    LogFunction(const Function::shared &arg_) : UnaryFunction("log", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::log(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::asin(arg->eval()); }
 protected:
    virtual shared _diff(shared v) const override {
        return std::make_shared<DivFunction>(arg->diff(v), arg);
    }
};

class ArcSinFunction : public UnaryFunction {
 public:
    ArcSinFunction(const Function::shared &arg_) : UnaryFunction("asin", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = std::make_shared<Constant>(std::asin(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::asin(arg->eval()); }
 protected:
    virtual shared _diff(shared v) const override {
        return std::make_shared<DivFunction>(
            arg->diff(v),
            std::make_shared<SquareRootFunction>(
                std::make_shared<SubFunction>(
                    std::make_shared<Constant>(1),
                    std::make_shared<MulFunction>(arg, arg))));
    }
};

inline Function::shared SinFunction::_diff(shared v) const {
    if (not FactoryBase::checkDepends(id(), v->id())) { return std::make_shared<Constant>(0); }
    return std::make_shared<CosFunction>(arg->diff(v));
}

inline Function::shared CosFunction::_diff(shared v) const {
    if (not FactoryBase::checkDepends(id(), v->id())) { return std::make_shared<Constant>(0); }
    return std::make_shared<NegFunction>(std::make_shared<SinFunction>(arg->diff(v)));
}

Function::shared operator -(const Function::shared &arg) {
    return std::make_shared<NegFunction>(arg);
}

Function::shared sin(const Function::shared &arg) {
    return std::make_shared<SinFunction>(arg);
}

Function::shared cos(const Function::shared &arg) {
    return std::make_shared<CosFunction>(arg);
}

Function::shared asin(const Function::shared &arg) {
    return std::make_shared<ArcSinFunction>(arg);
}

Function::shared sqrt(const Function::shared &arg) {
    return std::make_shared<SquareRootFunction>(arg);
}

Function::shared exp(const Function::shared &arg) {
    return std::make_shared<ExpFunction>(arg);
}

Function::shared log(const Function::shared &arg) {
    return std::make_shared<LogFunction>(arg);
}




}  // namespace sym

#endif  // UNARY_FUNCTION_HPP_
