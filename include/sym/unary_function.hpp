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

namespace sym {

class UnaryFunction : public Function {
 public:
    UnaryFunction(const std::string &operator_, const Symbol &arg_)
        : Function(_repr("(", operator_, "(", arg_->id(), "))"), {arg_->id()}), arg(arg_) {}

 protected:
    Symbol arg;
};

class ASExtractor;
class MDExtractor;
class NegFunction : public UnaryFunction {
 public:
    NegFunction(const Symbol &arg_) : UnaryFunction("-", arg_) {}
    virtual void simplified() const override;
    virtual double eval() const override { return -arg->eval(); }
 protected:
    virtual Symbol _diff(Symbol v) const override {
        return make_symbol<NegFunction>(arg->diff(v));
    }
    friend class ASExtractor;
    friend class MDExtractor;
};

class SinFunction : public UnaryFunction {
 public:
    SinFunction(const Symbol &arg_) : UnaryFunction("sin", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = make_symbol<Constant>(std::sin(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::sin(arg->eval()); }
 protected:
    virtual Symbol _diff(Symbol v) const override;
};

class CosFunction : public UnaryFunction {
 public:
    CosFunction(const Symbol &arg_) : UnaryFunction("cos", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = make_symbol<Constant>(std::cos(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::cos(arg->eval()); }
 protected:
    virtual Symbol _diff(Symbol v) const override;
};

class SquareRootFunction : public UnaryFunction {
 public:
    SquareRootFunction(const Symbol &arg_) : UnaryFunction("sqrt", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = make_symbol<Constant>(std::sqrt(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::sqrt(arg->eval()); }
 protected:
    virtual Symbol _diff(Symbol v) const override;
};

class ExpFunction : public UnaryFunction {
 public:
    ExpFunction(const Symbol &arg_) : UnaryFunction("exp", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = make_symbol<Constant>(std::exp(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::exp(arg->eval()); }
 protected:
    virtual Symbol _diff(Symbol v) const override;
};

class LogFunction : public UnaryFunction {
 public:
    LogFunction(const Symbol &arg_) : UnaryFunction("log", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = make_symbol<Constant>(std::log(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::log(arg->eval()); }
 protected:
    virtual Symbol _diff(Symbol v) const override;
};

class ArcSinFunction : public UnaryFunction {
 public:
    ArcSinFunction(const Symbol &arg_) : UnaryFunction("asin", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = make_symbol<Constant>(std::asin(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::asin(arg->eval()); }
 protected:
    virtual Symbol _diff(Symbol v) const override;
};

class ArcCosFunction : public UnaryFunction {
 public:
    ArcCosFunction(const Symbol &arg_) : UnaryFunction("acos", arg_) {}
    virtual void simplified() const override {
        arg->simplified();
        if (is_constant(arg)) {
            auto a = make_symbol<Constant>(std::acos(arg->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::acos(arg->eval()); }
 protected:
    virtual Symbol _diff(Symbol v) const override;
};

inline Symbol operator -(const Symbol &arg) {
    return make_symbol<NegFunction>(arg);
}

inline Symbol sin(const Symbol &arg) {
    return make_symbol<SinFunction>(arg);
}

inline Symbol cos(const Symbol &arg) {
    return make_symbol<CosFunction>(arg);
}

inline Symbol asin(const Symbol &arg) {
    return make_symbol<ArcSinFunction>(arg);
}

inline Symbol acos(const Symbol &arg) {
    return make_symbol<ArcCosFunction>(arg);
}

inline Symbol sqrt(const Symbol &arg) {
    return make_symbol<SquareRootFunction>(arg);
}

inline Symbol exp(const Symbol &arg) {
    return make_symbol<ExpFunction>(arg);
}

inline Symbol log(const Symbol &arg) {
    return make_symbol<LogFunction>(arg);
}

}  // namespace sym

#endif  // UNARY_FUNCTION_HPP_
