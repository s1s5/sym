/**
 * Copyright
 * @file binary_function.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:41:13
 */
#ifndef BINARY_FUNCTION_HPP_
#define BINARY_FUNCTION_HPP_

#include <cmath>

#include "function.hpp"

namespace sym {

class BinaryFunction : public Function {
 public:
    BinaryFunction(const std::string &operator_, const Symbol &arg0_, const Symbol &arg1_)
        : Function(_repr("(", arg0_->id(), operator_, arg1_->id(), ")"), {arg0_->id(), arg1_->id()}),
          arg0(arg0_),
          arg1(arg1_) {}

 protected:
    Symbol arg0, arg1;
};

class ASExtractor;
class AddFunction : public BinaryFunction {
 public:
    AddFunction(const Symbol &arg0, const Symbol &arg1) : BinaryFunction("+", arg0, arg1) {}
    virtual void simplified() const override;
    virtual double eval() const override { return arg0->eval() + arg1->eval(); }

 protected:
    virtual Symbol _diff(Symbol v) const override {
        return make_symbol<AddFunction>(arg0->diff(v), arg1->diff(v));
    }

    friend class ASExtractor;
};

class SubFunction : public BinaryFunction {
 public:
    SubFunction(const Symbol &arg0, const Symbol &arg1) : BinaryFunction("-", arg0, arg1) {}
    virtual void simplified() const override;
    virtual double eval() const override { return arg0->eval() - arg1->eval(); }

 protected:
    virtual Symbol _diff(Symbol v) const override {
        return make_symbol<SubFunction>(arg0->diff(v), arg1->diff(v));
    }

    friend class ASExtractor;
};

class MDExtractor;
class MulFunction : public BinaryFunction {
 public:
    MulFunction(const Symbol &arg0, const Symbol &arg1) : BinaryFunction("*", arg0, arg1) {}
    virtual void simplified() const override;
    virtual double eval() const override { return arg0->eval() * arg1->eval(); }

 protected:
    virtual Symbol _diff(Symbol v) const override {
        return make_symbol<AddFunction>(make_symbol<MulFunction>(arg0->diff(v), arg1),
                                        make_symbol<MulFunction>(arg0, arg1->diff(v)));
    }
    friend class MDExtractor;
};

class DivFunction : public BinaryFunction {
 public:
    DivFunction(const Symbol &arg) : BinaryFunction("/", one(), arg) {}

    virtual void simplified() const override;
    virtual double eval() const override { return 1 / arg1->eval(); }

 protected:
    virtual Symbol _diff(Symbol v) const override {
        auto d1 = arg1->diff(v);
        if (is_zero(d1)) {
            return zero();
        }
        return make_symbol<MulFunction>(
            make_symbol<MulFunction>(negative_one(), d1),
            make_symbol<DivFunction>(make_symbol<MulFunction>(arg1, arg1)));
    }
    friend class MDExtractor;
};

class Atan2Function : public BinaryFunction {
 public:
    Atan2Function(const Symbol &arg0_, const Symbol &arg1_) : BinaryFunction("atan2", arg0_, arg1_) {}
    virtual void simplified() const override {
        arg0->simplified();
        arg1->simplified();
        if (is_constant(arg0) and is_constant(arg1)) {
            auto a = make_symbol<Constant>(std::atan2(arg0->eval(), arg1->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::atan2(arg0->eval(), arg1->eval()); }

 protected:
    virtual Symbol _diff(Symbol v) const override {
        // return (f0->diff(var_id) * f1 - f0 * f1->diff(var_id)) / (f0 * f0 + f1 * f1);
        return make_symbol<SubFunction>(
            make_symbol<MulFunction>(arg0->diff(v), arg1),
            make_symbol<MulFunction>(make_symbol<MulFunction>(arg0, arg1->diff(v)),
                                     make_symbol<DivFunction>(
                                         make_symbol<AddFunction>(
                                             make_symbol<MulFunction>(arg0, arg0),
                                             make_symbol<MulFunction>(arg1, arg1)))));
    }
};

Symbol operator+(const Symbol &arg0, const Symbol &arg1) {
    return make_symbol<AddFunction>(arg0, arg1);
}

Symbol operator+(const double &arg0, const Symbol &arg1) {
    return make_symbol<AddFunction>(make_symbol<Constant>(arg0), arg1);
}

Symbol operator+(const Symbol &arg0, const double &arg1) {
    return make_symbol<AddFunction>(arg0, make_symbol<Constant>(arg1));
}

Symbol operator-(const Symbol &arg0, const Symbol &arg1) {
    return make_symbol<SubFunction>(arg0, arg1);
}

Symbol operator-(const double &arg0, const Symbol &arg1) {
    return make_symbol<SubFunction>(make_symbol<Constant>(arg0), arg1);
}

Symbol operator-(const Symbol &arg0, const double &arg1) {
    return make_symbol<SubFunction>(arg0, make_symbol<Constant>(arg1));
}

Symbol operator*(const Symbol &arg0, const Symbol &arg1) {
    return make_symbol<MulFunction>(arg0, arg1);
}

Symbol operator*(const double &arg0, const Symbol &arg1) {
    return make_symbol<MulFunction>(make_symbol<Constant>(arg0), arg1);
}

Symbol operator*(const Symbol &arg0, const double &arg1) {
    return make_symbol<MulFunction>(arg0, make_symbol<Constant>(arg1));
}

Symbol operator/(const Symbol &arg0, const Symbol &arg1) {
    return make_symbol<MulFunction>(arg0,
                                    make_symbol<DivFunction>(arg1));
}

Symbol operator/(const double &arg0, const Symbol &arg1) {
    return make_symbol<MulFunction>(make_symbol<Constant>(arg0),
                                    make_symbol<DivFunction>(arg1));
}

Symbol operator/(const Symbol &arg0, const double &arg1) {
    return make_symbol<MulFunction>(arg0,
                                    make_symbol<DivFunction>(make_symbol<Constant>(arg1)));
}

}  // namespace sym

#endif  // BINARY_FUNCTION_HPP_
