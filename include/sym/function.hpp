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
    // using shared = std::shared_ptr<Function>;
    
    class Symbol : public std::shared_ptr<Function> {
     public:
        Symbol(){}
        Symbol(const std::shared_ptr<Function> &f) : std::shared_ptr<Function>(f) {}
        Symbol(double v); //  : std::shared_ptr<Function>(_make_shared<Constant>(v)) {}

        Symbol & operator = (const double &v);
        template<class T>
        typename std::enable_if<std::is_base_of<Function, typename T::element_type>::value, Symbol &>::type
        operator=(const T &v) {
            std::shared_ptr<Function>::operator=(std::dynamic_pointer_cast<Function>(v));
            return *this;
        }

        Symbol & operator += (const double &v);
        Symbol & operator -= (const double &v);
        Symbol & operator += (const Symbol &v);
        Symbol & operator -= (const Symbol &v);
        Symbol & operator *= (const double &v);
        Symbol & operator /= (const double &v);
        Symbol & operator *= (const Symbol &v);
        Symbol & operator /= (const Symbol &v);
        
        bool operator == (const Symbol &rhs) const {
            return get()->id() == rhs->id();
        }
        
        bool operator != (const Symbol &rhs) const {
            return not (operator == (rhs));
        }

        std::string repr() const { return get()->repr(); }
        double eval() const { return get()->eval(); }
    };
    
 public:
    virtual ~Function() {}
    int id() const { return FactoryBase::alias(_id); }
    int orgId() const { return _id; }
    std::string repr() const { return FactoryBase::repr(_id); }

    virtual Symbol diff(Symbol v) const final;
    virtual void simplified() const = 0;
    virtual double eval() const = 0;

    template <class T>
    bool is() const {
        return FactoryBase::is<T>(id());
    }

    template <class T>
    std::shared_ptr<T> ptr() {
        return FactoryBase::ptr<T>(id());
    }

 protected:
    virtual Symbol _diff(Symbol v) const = 0;
    Function(const Repr &repr_, const std::unordered_set<int> &depends_) : _mem_repr(repr_), _mem_depends(depends_), _id(-1) {}

 private:
    void initId() {
        _id = FactoryBase::add(_mem_repr, _mem_depends, shared_from_this());
    }
    template<class T, class ...Args> friend std::shared_ptr<T> _make_shared(Args... args);

 private:
    Repr _mem_repr;
    std::unordered_set<int> _mem_depends;
    int _id;
};

template<class T, class ...Args>
std::shared_ptr<T> _make_shared(Args... args) {
    std::shared_ptr<T> r = std::make_shared<T>(args...);
    r->initId();
    r->simplified();
    return r;
}

using Symbol = Function::Symbol;

template<class T, class ...Args>
Symbol make_symbol(Args... args) {
    Symbol r;
    r = _make_shared<T>(args...);
    return r;
}

class Constant : public Function {
 public:
    Constant(double value_) : Function(_repr(std::to_string(value_)), {}), _value(value_) {}
    virtual void simplified() const override {}
    virtual double eval() const override { return _value; }

 protected:
    virtual Symbol _diff(Symbol v) const override { return make_symbol<Constant>(0); }

 protected:
    double _value;
};

inline bool is_zero(const Symbol &f) {
    static const std::string zero_string = std::to_string(0.0);
    return f->repr() == zero_string;
}

inline bool is_one(const Symbol &f) {
    static const std::string one_string = std::to_string(1.0);
    return f->repr() == one_string;
}

inline bool is_negative_one(const Symbol &f) {
    static const std::string negative_one_string = std::to_string(-1.0);
    return f->repr() == negative_one_string;
}

inline bool is_constant(const Symbol &f) {
    return FactoryBase::is<Constant>(f->id());
}

inline Symbol zero() {
    return make_symbol<Constant>(0);
}

inline Symbol one() {
    return make_symbol<Constant>(1);
}

inline Symbol negative_one() {
    return make_symbol<Constant>(-1);
}

class Variable : public Function {
 public:
    Variable(const std::string &symbol) : Function(_repr(symbol), {}) {}
    virtual void simplified() const override {}
    virtual double eval() const override { return _value; }
    void assign(double v) { _value = v; }

 protected:
    virtual Symbol _diff(Symbol v) const override {
        if (v->id() == id()) { return make_symbol<Constant>(1); }
        return make_symbol<Constant>(0);
    }

 protected:
    double _value{0};
};

inline Function::Symbol Function::diff(Symbol v) const {
    if (id() == v->id()) {
        return make_symbol<Constant>(1);
    } else if (not FactoryBase::checkDepends(id(), v->id())) {
        return make_symbol<Constant>(0);
    }
    auto result = _diff(v);
    result->simplified();
    return result;
}

inline Function::Symbol::Symbol(double v) : std::shared_ptr<Function>(_make_shared<Constant>(v)) {}
inline Function::Symbol & Function::Symbol::operator = (const double &v) { *this = Symbol(v); return *this; }

inline std::ostream &operator<<(std::ostream &os, const Symbol &arg) {
    if (not arg) { return os << std::string("<null>"); }
    return os << arg->repr();
}

// template<class T0, class T1>
// typename std::enable_if<std::is_base_of<Function, typename T0>::value
//                         and std::is_base_of<Function, typename T1>::value, bool>::type
// operator == (const std::shared_ptr<T0> &arg0, const std::shared_ptr<T1> &arg1) {
//     std::cout << arg0 << " == " << arg1 << " -> " << FactoryBase::alias(arg0->id()) << "==" << FactoryBase::alias(arg1->id()) << std::endl;
//     return FactoryBase::alias(arg0->id()) == FactoryBase::alias(arg1->id());
// }

// template<class T0, class T1>
// typename std::enable_if<std::is_base_of<Function, typename T0::element_type>::value
//                         and std::is_base_of<Function, typename T1::element_type>::value, bool>::type
// operator != (const T0 &arg0, const T1 &arg1) {
//     return not (arg0 == arg1);
// }

}  // namespace sym

#endif  // FUNCTION_HPP_
