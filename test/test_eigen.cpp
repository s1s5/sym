/**
 * Copyright 
 * @file test_eigen.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 13:13:48
 */
#include "cpput.hpp"

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


namespace {

using namespace sym;

struct eigen : public Factory {
    StaticInput x0{"x0", 4};
    StaticInput x1{"x1", 4};
    StaticOutput y{"y", 3};
};

TEST_F(eigen, 000) {
    Eigen::Matrix<Symbol, 2, 2> m0, m1;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            m0(i, j) = x0[2 * i + j];
            m1(i, j) = x1[2 * i + j];
        }
    }
    m0(1, 1) = 0;
    std::cout << m0 << std::endl;
    std::cout << m0 * m1 << std::endl;
}

}  // namespace
