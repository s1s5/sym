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
#include "as_extractor.hpp"

namespace sym {

inline void NegFunction::simplified() const {
    arg->simplified();
    if (is_zero(arg)) {
        FactoryBase::setAliasRepr(id(), arg->id());
    } else if (is_constant(arg)) {
        auto a = make_symbol<Constant>(- arg->eval());
        FactoryBase::setAliasRepr(id(), a->id());
    }
    ASExtractor ex;
    ex.addSub(arg);
    if (ex.canBeSimplified()) {
        auto a = ex.simplified();
        FactoryBase::setAliasRepr(id(), a->id());
    }
}

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
    return make_symbol<MulFunction>(arg->diff(v), 
                                    make_symbol<DivFunction>(
                                        make_symbol<MulFunction>(
                                            make_symbol<Constant>(2),
                                            make_symbol<SquareRootFunction>(arg))));
}

inline Symbol ExpFunction::_diff(Symbol v) const {
    return make_symbol<MulFunction>(arg->diff(v), make_symbol<ExpFunction>(arg));
}

inline Symbol LogFunction::_diff(Symbol v) const {
    return make_symbol<MulFunction>(arg->diff(v),
                                    make_symbol<DivFunction>(arg));
}

inline Symbol ArcSinFunction::_diff(Symbol v) const {
    return make_symbol<MulFunction>(arg->diff(v),
                                    make_symbol<DivFunction>(
                                        make_symbol<SquareRootFunction>(
                                            make_symbol<SubFunction>(
                                                make_symbol<Constant>(1),
                                                make_symbol<MulFunction>(arg, arg)))));
}

inline Symbol ArcCosFunction::_diff(Symbol v) const {
    return make_symbol<NegFunction>(
        make_symbol<MulFunction>(arg->diff(v),
                                 make_symbol<DivFunction>(
                                     make_symbol<SquareRootFunction>(
                                         make_symbol<SubFunction>(
                                             make_symbol<Constant>(1),
                                             make_symbol<MulFunction>(arg, arg))))));
}

}  // namespace sym

#endif  // UNARY_FUNCTION_IMPL_HPP_
