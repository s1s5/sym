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
    BinaryFunction(const std::string &operator_, const Function::shared &arg0_, const Function::shared &arg1_)
        : Function(_repr("(", arg0_->id(), operator_, arg1_->id(), ")"),
                   FactoryBase::depends(arg0_->id(), arg1_->id())),
          arg0(arg0_),
          arg1(arg1_) {}

 protected:
    Function::shared arg0, arg1;
};

class AddFunction : public BinaryFunction {
 public:
    AddFunction(const Function::shared &arg0, const Function::shared &arg1) : BinaryFunction("+", arg0, arg1) {}
    virtual void simplified() const override {
        arg0->simplified();
        arg1->simplified();
        if (is_zero(arg0)) {
            FactoryBase::setAliasRepr(id(), arg1->id());
        } else if (is_zero(arg1)) {
            FactoryBase::setAliasRepr(id(), arg0->id());
        }
    }
    virtual double eval() const override { return arg0->eval() + arg1->eval(); }

 protected:
    virtual shared _diff(shared v) const override {
        return _make_shared<AddFunction>(arg0->diff(v), arg1->diff(v));
    }
};

class SubFunction : public BinaryFunction {
 public:
    SubFunction(const Function::shared &arg0, const Function::shared &arg1) : BinaryFunction("-", arg0, arg1) {}
    virtual void simplified() const override;
    virtual double eval() const override { return arg0->eval() - arg1->eval(); }

 protected:
    virtual shared _diff(shared v) const override {
        return _make_shared<SubFunction>(arg0->diff(v), arg1->diff(v));
    }
};

class MulFunction : public BinaryFunction {
 public:
    MulFunction(const Function::shared &arg0, const Function::shared &arg1) : BinaryFunction("*", arg0, arg1) {}
    virtual void simplified() const override {
        arg0->simplified();
        arg1->simplified();
        if (is_zero(arg0) or is_zero(arg1)) {
            FactoryBase::setAliasRepr(id(), zero()->id());
        } else if (is_one(arg0)) {
            FactoryBase::setAliasRepr(id(), arg1->id());
        } else if (is_one(arg1)) {
            FactoryBase::setAliasRepr(id(), arg0->id());
        }
    }
    virtual double eval() const override { return arg0->eval() * arg1->eval(); }

 protected:
    virtual shared _diff(shared v) const override {
        return _make_shared<AddFunction>(_make_shared<MulFunction>(arg0->diff(v), arg1),
                                             _make_shared<MulFunction>(arg0, arg1->diff(v)));
    }
};

class DivFunction : public BinaryFunction {
 public:
    DivFunction(const Function::shared &arg0, const Function::shared &arg1) : BinaryFunction("/", arg0, arg1) {}

    virtual void simplified() const override {
        arg0->simplified();
        arg1->simplified();
        if (is_zero(arg0)) {
            FactoryBase::setAliasRepr(id(), arg0->id()); 
        } else if (is_one(arg1)) {
            FactoryBase::setAliasRepr(id(), arg0->id());
        }
    }
    virtual double eval() const override { return arg0->eval() / arg1->eval(); }

 protected:
    virtual shared _diff(shared v) const override {
        auto d0 = arg0->diff(v);
        auto d1 = arg1->diff(v);
        if (is_zero(d1)) {
            return _make_shared<DivFunction>(d0, arg1);
        }
        return _make_shared<SubFunction>(
            _make_shared<DivFunction>(d0, arg1),
            _make_shared<DivFunction>(_make_shared<DivFunction>(arg0, d1),
                                      _make_shared<MulFunction>(arg1, arg1)));
    }
};

class Atan2Function : public BinaryFunction {
 public:
    Atan2Function(const Function::shared &arg0_, const Function::shared &arg1_) : BinaryFunction("atan2", arg0_, arg1_) {}
    virtual void simplified() const override {
        arg0->simplified();
        arg1->simplified();
        if (is_constant(arg0) and is_constant(arg1)) {
            auto a = _make_shared<Constant>(std::atan2(arg0->eval(), arg1->eval()));
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
    virtual double eval() const override { return std::atan2(arg0->eval(), arg1->eval()); }

 protected:
    virtual shared _diff(shared v) const override {
        // return (f0->diff(var_id) * f1 - f0 * f1->diff(var_id)) / (f0 * f0 + f1 * f1);
        return _make_shared<SubFunction>(
            _make_shared<MulFunction>(arg0->diff(v), arg1),
            _make_shared<DivFunction>(
                _make_shared<MulFunction>(arg0, arg1->diff(v)),
                _make_shared<AddFunction>(
                    _make_shared<MulFunction>(arg0, arg0),
                    _make_shared<MulFunction>(arg1, arg1))));
    }
};

Function::shared operator+(const Function::shared &arg0, const Function::shared &arg1) {
    return _make_shared<AddFunction>(arg0, arg1);
}

Function::shared operator+(const double &arg0, const Function::shared &arg1) {
    return _make_shared<AddFunction>(_make_shared<Constant>(arg0), arg1);
}

Function::shared operator+(const Function::shared &arg0, const double &arg1) {
    return _make_shared<AddFunction>(arg0, _make_shared<Constant>(arg1));
}

Function::shared operator-(const Function::shared &arg0, const Function::shared &arg1) {
    return _make_shared<SubFunction>(arg0, arg1);
}

Function::shared operator-(const double &arg0, const Function::shared &arg1) {
    return _make_shared<SubFunction>(_make_shared<Constant>(arg0), arg1);
}

Function::shared operator-(const Function::shared &arg0, const double &arg1) {
    return _make_shared<SubFunction>(arg0, _make_shared<Constant>(arg1));
}

Function::shared operator*(const Function::shared &arg0, const Function::shared &arg1) {
    return _make_shared<MulFunction>(arg0, arg1);
}

Function::shared operator*(const double &arg0, const Function::shared &arg1) {
    return _make_shared<MulFunction>(_make_shared<Constant>(arg0), arg1);
}

Function::shared operator*(const Function::shared &arg0, const double &arg1) {
    return _make_shared<MulFunction>(arg0, _make_shared<Constant>(arg1));
}

Function::shared operator/(const Function::shared &arg0, const Function::shared &arg1) {
    return _make_shared<DivFunction>(arg0, arg1);
}

Function::shared operator/(const double &arg0, const Function::shared &arg1) {
    return _make_shared<DivFunction>(_make_shared<Constant>(arg0), arg1);
}

Function::shared operator/(const Function::shared &arg0, const double &arg1) {
    return _make_shared<DivFunction>(arg0, _make_shared<Constant>(arg1));
}

}  // namespace sym

#endif  // BINARY_FUNCTION_HPP_
