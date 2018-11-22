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

Function::shared SquareRootFunction::_diff(shared v) const {
    return _make_shared<MulFunction>(
        _make_shared<Constant>(0.5),
        _make_shared<DivFunction>(arg->diff(v), arg));
}

Function::shared ExpFunction::_diff(shared v) const {
    return _make_shared<MulFunction>(arg->diff(v), arg);
}

Function::shared LogFunction::_diff(shared v) const {
    return _make_shared<DivFunction>(arg->diff(v), arg);
}

Function::shared ArcSinFunction::_diff(shared v) const {
    return _make_shared<DivFunction>(
        arg->diff(v),
        _make_shared<SquareRootFunction>(
            _make_shared<SubFunction>(
                _make_shared<Constant>(1),
                _make_shared<MulFunction>(arg, arg))));
}

}  // namespace sym

#endif  // UNARY_FUNCTION_IMPL_HPP_
