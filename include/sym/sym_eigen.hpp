/**
 * Copyright 
 * @file sym_eigen.hpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 15:28:32
 */
#ifndef SYM_EIGEN_HPP_
#define SYM_EIGEN_HPP_

#include "sym/sym.hpp"

namespace Eigen {
template<> struct NumTraits<sym::Symbol> : NumTraits<int> {
    using Real = sym::Symbol;
    using NonInteger = sym::Symbol;
    using NonNested = sym::Symbol;
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

template<int R, int C, int Major = Eigen::ColMajor>
Eigen::Map<const Eigen::Matrix<Symbol, R, C, Major>> asMatrix(const Input &input) {
    if (R * C > input.size()) {
        throw std::runtime_error("size mismatch");
    }
    return Eigen::Map<const Eigen::Matrix<Symbol, R, C, Major>>(input.data());
}

template<int R, int C, int Major = Eigen::ColMajor>
Eigen::Map<const Eigen::Matrix<Symbol, R, C, Major>> asMatrix(const Output &output) {
    if (R * C > output.size()) {
        throw std::runtime_error("size mismatch");
    }
    return Eigen::Map<const Eigen::Matrix<Symbol, R, C, Major>>(output.data());
}

template<int R, int C, int Major = Eigen::ColMajor>
Eigen::Map<Eigen::Matrix<Symbol, R, C, Major>> asMatrix(Output &output) {
    if (R * C > output.size()) {
        throw std::runtime_error("size mismatch");
    }
    return Eigen::Map<Eigen::Matrix<Symbol, R, C, Major>>(output.data());
}

template<int N>
Eigen::Map<const Eigen::Matrix<Symbol, N, 1>> asVector(const Input &input) {
    if (N > input.size()) {
        throw std::runtime_error("size mismatch");
    }
    return Eigen::Map<const Eigen::Matrix<Symbol, N, 1>>(input.data());
}

template<int N>
Eigen::Map<const Eigen::Matrix<Symbol, N, 1>> asVector(const Output &output) {
    if (N > output.size()) {
        throw std::runtime_error("size mismatch");
    }
    return Eigen::Map<const Eigen::Matrix<Symbol, N, 1>>(output.data());
}

template<int N>
Eigen::Map<Eigen::Matrix<Symbol, N, 1>> asVector(Output &output) {
    if (N > output.size()) {
        throw std::runtime_error("size mismatch");
    }
    return Eigen::Map<Eigen::Matrix<Symbol, N, 1>>(output.data());
}

inline double eval(const Symbol &func) {
    return func->eval();
}
}  // namespace sym

#endif  // SYM_EIGEN_HPP_
