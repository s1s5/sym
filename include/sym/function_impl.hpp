/**
 * Copyright 
 * @file function_impl.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 15:48:19
 */
#ifndef FUNCTION_IMPL_HPP_
#define FUNCTION_IMPL_HPP_

#include "function.hpp"
#include "binary_function.hpp"

namespace sym {

inline Function::Symbol & Function::Symbol::operator += (const double &v) {
    return *this = (*this) + v;
}

inline Function::Symbol & Function::Symbol::operator -= (const double &v) {
    return *this = (*this) - v;
}

inline Function::Symbol & Function::Symbol::operator += (const Symbol &v) {
    return *this = (*this) + v;
}

inline Function::Symbol & Function::Symbol::operator -= (const Symbol &v) {
    return *this = (*this) - v;
}


}  // namespace sym


#endif  // FUNCTION_IMPL_HPP_
