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

namespace sym {

#define DEF_SYM_SQUARE_MATRIX(N) using Matrix ## N ## s = Eigen::Matrix<Symbol, N, N>
DEF_SYM_SQUARE_MATRIX(1);
DEF_SYM_SQUARE_MATRIX(2);
DEF_SYM_SQUARE_MATRIX(3);
DEF_SYM_SQUARE_MATRIX(4);
#undef DEF_SYM_SQUARE_MATRIX

#define DEF_SYM_VECTOR(N) using Vector ## N ## s = Eigen::Matrix<Symbol, N, 1>
DEF_SYM_VECTOR(1);
DEF_SYM_VECTOR(2);
DEF_SYM_VECTOR(3);
DEF_SYM_VECTOR(4);
#undef DEF_SYM_VECTOR

}  // namespace sym

#endif  // SYM_EIGEN_HPP_
