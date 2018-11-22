/**
 * Copyright
 * @file function.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:40:20
 */
#ifndef FUNCTION_HPP_
#define FUNCTION_HPP_

#include "factory_base.hpp"

namespace sym {

class Function : public std::enable_shared_from_this<Function> {
 public:
    using shared = std::shared_ptr<Function>;

 public:
    virtual ~Function() {}
    int id() const { return _id; }
    std::string repr() { return FactoryBase::repr(_id); }

    virtual shared diff(shared v) const final;
    virtual void simplified() const = 0;
    virtual double eval() const = 0;

    template <class T>
    bool is() const {
        return dynamic_cast<const T *>(this);
    }

 protected:
    virtual shared _diff(shared v) const = 0;
    Function(const Repr &repr, const std::unordered_set<int> &depends) : _id(FactoryBase::add(repr, depends, this)) {}

 private:
    int _id;
};

class Constant : public Function {
 public:
    Constant(double value_) : Function(_repr(std::to_string(value_)), {}), _value(value_) {}
    virtual void simplified() const override {}
    virtual double eval() const override { return _value; }

 protected:
    virtual shared _diff(shared v) const override { return std::make_shared<Constant>(0); }

 protected:
    double _value;
};

inline bool is_zero(const Function::shared &f) {
    static const std::string zero_string = std::to_string(0.0);
    return f->repr() == zero_string;
}

inline bool is_one(const Function::shared &f) {
    static const std::string one_string = std::to_string(1.0);
    return f->repr() == one_string;
}

inline bool is_constant(const Function::shared &f) {
    return FactoryBase::is<Constant>(f->id());
}

inline Function::shared zero() {
    return std::make_shared<Constant>(0);
}

inline Function::shared one() {
    return std::make_shared<Constant>(1);
}

class Variable : public Function {
 public:
    Variable(const std::string &symbol) : Function(_repr(symbol), {}) {}
    virtual void simplified() const override {}
    virtual double eval() const override { return _value; }
    void assign(double v) { _value = v; }

 protected:
    virtual shared _diff(shared v) const override {
        if (v->id() == id()) { return std::make_shared<Constant>(1); }
        return std::make_shared<Constant>(0);
    }

 protected:
    double _value{0};
};

Function::shared Function::diff(shared v) const {
    if (id() == v->id()) {
        return std::make_shared<Constant>(1);
    } else if (not FactoryBase::checkDepends(id(), v->id())) {
        return std::make_shared<Constant>(0);
    }
    auto result = _diff(v);
    result->simplified();
    return result;
}

inline bool operator == (const Function::shared &arg0, const Function::shared &arg1) {
    return FactoryBase::alias(arg0->id()) == FactoryBase::alias(arg1->id());
}

inline bool operator != (const Function::shared &arg0, const Function::shared &arg1) {
    return not (arg0 == arg1);
}

inline std::ostream &operator<<(std::ostream &os, const Function::shared &arg) {
    if (not arg) { return os << std::string("<null>"); }
    return os << arg->repr();
}

}  // namespace sym

#endif  // FUNCTION_HPP_
