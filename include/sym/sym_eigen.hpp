/**
 * Copyright 
 * @file sym_eigen.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 15:28:32
 */
#ifndef SYM_EIGEN_HPP_
#define SYM_EIGEN_HPP_

#include <Eigen/Eigen>
#include "sym/sym.hpp"

namespace Eigen {
template<> struct NumTraits<sym::Symbol> : NumTraits<int32_t> {
    enum {
        IsComplex = 0,
        IsInteger = 1,
        IsSigned = 1,
        RequireInitialization = 1,
        ReadCost = 1,
        AddCost = 3,
        MulCost = 3
    };
};
}  // namespace Eigen


#endif  // SYM_EIGEN_HPP_
