/**
 * Copyright 
 * @file unary_function_impl.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 10:46:56
 */
#ifndef UNARY_FUNCTION_IMPL_HPP_
#define UNARY_FUNCTION_IMPL_HPP_

#include "unary_function.hpp"
#include "binary_function.hpp"

namespace sym {

inline Symbol SinFunction::_diff(Symbol v) const {
    return make_symbol<MulFunction>(
        arg->diff(v),
        make_symbol<CosFunction>(arg));
}

inline Symbol CosFunction::_diff(Symbol v) const {
    return make_symbol<MulFunction>(
        make_symbol<NegFunction>(arg->diff(v)),
        make_symbol<SinFunction>(arg));
}

inline Symbol SquareRootFunction::_diff(Symbol v) const {
    return make_symbol<DivFunction>(
        arg->diff(v), 
        make_symbol<MulFunction>(
            make_symbol<Constant>(2),
            make_symbol<SquareRootFunction>(arg)));
}

inline Symbol ExpFunction::_diff(Symbol v) const {
    return make_symbol<MulFunction>(arg->diff(v), make_symbol<ExpFunction>(arg));
}

inline Symbol LogFunction::_diff(Symbol v) const {
    return make_symbol<DivFunction>(arg->diff(v), arg);
}

inline Symbol ArcSinFunction::_diff(Symbol v) const {
    return make_symbol<DivFunction>(
        arg->diff(v),
        make_symbol<SquareRootFunction>(
            make_symbol<SubFunction>(
                make_symbol<Constant>(1),
                make_symbol<MulFunction>(arg, arg))));
}

}  // namespace sym

#endif  // UNARY_FUNCTION_IMPL_HPP_
