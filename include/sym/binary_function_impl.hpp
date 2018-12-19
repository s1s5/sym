/**
 * Copyright 
 * @file binary_function_impl.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 10:45:47
 */
#ifndef BINARY_FUNCTION_IMPL_HPP_
#define BINARY_FUNCTION_IMPL_HPP_

#include "binary_function.hpp"
#include "unary_function.hpp"
#include "as_extractor.hpp"
#include "md_extractor.hpp"

namespace sym {

inline void AddFunction::simplified() const  {
    if (is_zero(arg0)) {
        FactoryBase::setAliasRepr(id(), arg1->id());
    } else if (is_zero(arg1)) {
        FactoryBase::setAliasRepr(id(), arg0->id());
    } else if (is_constant(arg0) and is_constant(arg1)) {
        auto a = make_symbol<Constant>(arg0.eval() + arg1.eval());
        FactoryBase::setAliasRepr(id(), a->id());
    } else if (is_constant(arg1)) {
        auto a = make_symbol<AddFunction>(arg1, arg0);
        FactoryBase::setAliasRepr(id(), a->id());
    } else {
        // arg0->simplified();
        // arg1->simplified();
        ASExtractor ex;
        ex.addAdd(arg0);
        ex.addAdd(arg1);
        if (ex.canBeSimplified()) {
            auto a = ex.simplified();
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
}

inline void SubFunction::simplified() const {
    // arg0->simplified();
    // arg1->simplified();
    if (is_zero(arg0)) {
        auto a = make_symbol<NegFunction>(arg1);
        FactoryBase::setAliasRepr(id(), a->id());
    } else if (is_zero(arg1)) {
        FactoryBase::setAliasRepr(id(), arg0->id());
    } else if (is_constant(arg0) and is_constant(arg1)) {
        auto a = make_symbol<Constant>(arg0.eval() - arg1.eval());
        FactoryBase::setAliasRepr(id(), a->id());
    } else if (is_constant(arg1)) {
        auto a = make_symbol<AddFunction>(Symbol(- arg1.eval()), arg0);
        FactoryBase::setAliasRepr(id(), a->id());
    } else {
        // std::cout << arg0 << " - " << arg1 << std::endl;
        ASExtractor ex;
        ex.addAdd(arg0);
        ex.addSub(arg1);
        if (ex.canBeSimplified()) {
            auto a = ex.simplified();
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
}

inline void MulFunction::simplified() const  {
    // arg0->simplified();
    // arg1->simplified();
    if (is_zero(arg0) or is_zero(arg1)) {
        FactoryBase::setAliasRepr(id(), zero()->id());
    } else if (is_one(arg0)) {
        FactoryBase::setAliasRepr(id(), arg1->id());
    } else if (is_one(arg1)) {
        FactoryBase::setAliasRepr(id(), arg0->id());
    } else if (is_constant(arg0) and is_constant(arg1)) {
        Symbol a(arg0->eval() * arg1->eval());
        FactoryBase::setAliasRepr(id(), a->id());
    } else if (is_negative_one(arg0)) {
        auto a = make_symbol<NegFunction>(arg1);
        FactoryBase::setAliasRepr(id(), a->id());
    } else if (is_negative_one(arg0)) {
        auto a = make_symbol<NegFunction>(arg0);
        FactoryBase::setAliasRepr(id(), a->id());
    } else if (is_constant(arg0) and is_constant(arg1)) {
        auto a = make_symbol<Constant>(arg0.eval() * arg1.eval());
        FactoryBase::setAliasRepr(id(), a->id());
    } else if (is_constant(arg1)) {
        auto a = make_symbol<MulFunction>(arg1, arg0);
        FactoryBase::setAliasRepr(id(), a->id());
    } else {
        MDExtractor mx;
        mx.addMul(arg0);
        mx.addMul(arg1);
        if (mx.canBeSimplified()) {
            auto a = mx.simplified();
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
}

void DivFunction::simplified() const  {
    // arg0->simplified();
    // arg1->simplified();
    if (is_zero(arg0)) {
        FactoryBase::setAliasRepr(id(), arg0->id()); 
    } else if (is_one(arg1)) {
        FactoryBase::setAliasRepr(id(), arg0->id());
    } else {
        MDExtractor mx;
        mx.addDiv(arg1);
        if (mx.canBeSimplified()) {
            auto a = mx.simplified();
            FactoryBase::setAliasRepr(id(), a->id());
        }
    }
}

}  // namespace sym


#endif  // BINARY_FUNCTION_IMPL_HPP_
