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

namespace sym {

inline void SubFunction::simplified() const {
    arg0->simplified();
    arg1->simplified();
    if (is_zero(arg0)) {
        auto a = _make_shared<NegFunction>(arg1);
        FactoryBase::setAliasRepr(id(), a->id());
    } else if (is_zero(arg1)) {
        FactoryBase::setAliasRepr(id(), arg0->id());
    }
}
}  // namespace sym


#endif  // BINARY_FUNCTION_IMPL_HPP_
