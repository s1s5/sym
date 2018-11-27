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
#include "sym/sym_eigen.hpp"

namespace {

using namespace sym;

struct eigen : public Factory {
    StaticInput x0{"x0", 4};
    StaticInput x1{"x1", 4};
    StaticOutput y{"y", 3};
};

TEST_F(eigen, 000) {
    Matrix2s m0, m1;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            m0(i, j) = x0[2 * i + j];
            m1(i, j) = x1[2 * i + j];
        }
    }
    m0(1, 1) = 0;
    // std::cout << m0 << std::endl;
    // std::cout << m0 * m1 << std::endl;

    ASSERT_EQ(m0(0, 0), x0[0]);

    Matrix2s m01 = m0 * m1;
    ASSERT_EQ(m01(0, 0), x0[0] * x1[0] + x0[1] * x1[2]);
}

TEST_F(eigen, 001) {
    Eigen::AngleAxis<Symbol> angle(x0[0], Eigen::Matrix<Symbol, 3, 1>(0, 0, 1));
    // std::cout << "--------------------------------------------------------------------------------" << std::endl;
    // std::cout << angle.toRotationMatrix() << std::endl;
    Matrix3s r = angle.toRotationMatrix();
    ASSERT_EQ(r(0, 0), cos(x0[0]));
    ASSERT_EQ(r(2, 2), 1);
}

TEST_F(eigen, 002) {
    {
        Eigen::Map<const Matrix2s> m(x0.data());
        // std::cout << m << std::endl;
        // col major
        ASSERT_EQ(m(0, 0), x0[0]);
        ASSERT_EQ(m(1, 0), x0[1]);
        ASSERT_EQ(m(0, 1), x0[2]);
        ASSERT_EQ(m(1, 1), x0[3]);
    }
    {
        auto m = asMatrix<2, 2>(x0);
        // std::cout << m << std::endl;
        // col major
        ASSERT_EQ(m(0, 0), x0[0]);
        ASSERT_EQ(m(1, 0), x0[1]);
        ASSERT_EQ(m(0, 1), x0[2]);
        ASSERT_EQ(m(1, 1), x0[3]);
    }
}

TEST_F(eigen, 003) {
    Eigen::Vector4d r(1, 2, 3, 4);
    x0.assign({r(0), r(1), r(2), r(3), });
    auto v = asVector<4>(x0);
    // std::cout << v.transpose() << std::endl;
    // std::cout << v.normalized().transpose() << std::endl;
    // std::cout << (v.unaryExpr(&eval) - r) << std::endl;
    // std::cout << (v.normalized().unaryExpr(&eval) - r.normalized()) << std::endl;
    ASSERT_NEAR((v.unaryExpr(&eval) - r).norm(), 0, 1.0e-8);
    ASSERT_NEAR((v.normalized().unaryExpr(&eval) - r.normalized()).norm(), 0, 1.0e-8);
}

}  // namespace
