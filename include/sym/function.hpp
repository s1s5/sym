/**
 * Copyright 
 * @file function.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:40:20
 */
#ifndef FUNCTION_HPP_
#define FUNCTION_HPP_

#include <memory>
#include "factory_base.hpp"

namespace sym {

class Function : public std::enable_shared_from_this<Function> {
 public:
    using shared = std::shared_ptr<Function>;

 public:
    virtual ~Function() {}
    int id() const { return _id; }
    const std::string &repr() { return FactoryBase::repr(_id); }
    virtual shared diff(shared v) const = 0;
    virtual void simplified() const = 0;

    template<class T>
    bool is() const { return dynamic_cast<const T*>(this); }

 protected:
    Function(const std::string &repr, const std::unordered_set<int> &depends) : _id(FactoryBase::add(repr, depends)) {
    }
    
 private:
    int _id;
};

class Constant : public Function {
 public:
    Constant(double value) : Function(std::to_string(value), {}) {}
    virtual shared diff(shared v) const override { return std::make_shared<Constant>(0); }
    virtual void simplified() const override {}
};

inline bool is_zero(const Function::shared &f) {
    static const std::string zero_string = std::to_string(0.0);
    return f->repr() == zero_string;
}

inline bool is_one(const Function::shared &f) {
    static const std::string one_string = std::to_string(1.0);
    return f->repr() == one_string;
}

class Variable : public Function {
 public:
    Variable(const std::string &symbol) : Function(symbol, {}) {}
    virtual shared diff(shared v) const override {
        if (v->id() == id()) {
            return std::make_shared<Constant>(1);
        }
        return std::make_shared<Constant>(0);
    }
    virtual void simplified() const override {}
};

std::ostream &operator << (std::ostream &os, const Function::shared &arg) {
    if (not arg) {
        return os << std::string("<null>");
    }
    return os << arg->repr();
}

}  // namespace sym

#endif  // FUNCTION_HPP_
