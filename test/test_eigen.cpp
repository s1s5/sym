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
#if 0
TEST_F(eigen, 000) {
    Matrix2 m0, m1;
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

    Matrix2 m01 = m0 * m1;
    ASSERT_EQ(m01(0, 0), x0[0] * x1[0] + x0[1] * x1[2]);
}

TEST_F(eigen, 001) {
    Eigen::AngleAxis<Symbol> angle(x0[0], Vector3(0, 0, 1));
    // std::cout << "--------------------------------------------------------------------------------" << std::endl;
    // std::cout << angle.toRotationMatrix() << std::endl;
    Matrix3 r = angle.toRotationMatrix();
    ASSERT_EQ(r(0, 0), cos(x0[0]));
    ASSERT_EQ(r(2, 2), 1);
}

TEST_F(eigen, 002) {
    {
        Eigen::Map<const Matrix2> m(x0.data());
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

TEST_F(eigen, 004) {
    Vector3 r(x0[0], x0[1], 2);
    r += Vector3(1, 2, 3);
    r += Vector3(1, 2, 3);
    std::cout << r.transpose() << std::endl;
}

TEST_F(eigen, 005) {
    Vector3 r(x0[0], x0[1], 2);
    r.head<2>() += Vector2(1, 2);
    std::cout << r.transpose() << std::endl;
}

TEST_F(eigen, 006) {
    Matrix3 R = Matrix3::Identity();
    Vector3 r0(x0[0], x0[1], x0[2]), r1(x1[0], x1[1], x1[2]);
    auto Rr0 = R * r0 + r1;
    std::cout << Rr0.transpose() << std::endl;
    auto r2 = r0 + Rr0;
    std::cout << typeid(r2).name() << " : " << r2.transpose() << std::endl;
}

TEST_F(eigen, 007) {
    Eigen::Transform<Symbol, 3, Eigen::Isometry> iso = Eigen::Transform<Symbol, 3, Eigen::Isometry>::Identity();
    auto t = iso.linearExt() * Vector3(x0[0], x0[1], x0[2]);
    std::cout << t << std::endl;
    // iso.translate(Vector3(x0[0], x0[1], x0[2]));
}

TEST_F(eigen, 008) {
    Eigen::AngleAxis<Symbol> Rx(x0[0], Vector3(1, 0, 0));
    Eigen::AngleAxis<Symbol> Ry(x0[1], Vector3(0, 1, 0));
    Eigen::AngleAxis<Symbol> Rz(x0[2], Vector3(0, 0, 1));
    auto R = Rz * Rx * Ry;
    auto v = R * Vector3(x1[0], x1[1], x1[2]) + Vector3(x0[0], x0[1], x0[2]);
    auto d = diff(v(0), x0[0]);
    std::cout << d << std::endl;
}

TEST_F(eigen, 009) {
    Eigen::AngleAxis<Symbol> Rx(x0[0], Vector3(1, 0, 0));
    Eigen::AngleAxis<Symbol> Ry(x0[1], Vector3(0, 1, 0));
    // Eigen::AngleAxis<Symbol> Rz(x0[2], Vector3(0, 0, 1));
    // auto R = (Rz.toRotationMatrix() * Rx.toRotationMatrix() * Ry.toRotationMatrix());
    auto R = (Rx.toRotationMatrix() * Ry.toRotationMatrix());
    // Vector3 xyz_0((x1[0] - x0[0]) / x1[2], (x1[1] - x0[1]) / x1[2], x0[0] / x1[2]);
    // Vector3 xyz_i = R * xyz_0 + Vector3(x0[3], x1[3], x0[3]);
    std::cout << typeid(R).name() << std::endl;
    std::cout << R(0, 0) << std::endl;
    // Vector3 xyz_i = (R) * Vector3(x1[0], x1[1], x1[2]) + Vector3(x0[3], x1[3], x0[3]);
    // Vector3 xyz_i = R * Vector3(x1[0], x1[1], x1[2]);
    // std::cout << xyz_i.transpose() << std::endl;
    // Vector2 ixy(x0[0] * xyz_i(0) / xyz_i(2) + x0[0], x0[0] * xyz_i(1) / xyz_i(2) + x0[1]);
    // Eigen::Matrix<Symbol, 2, 6> J;
    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 6; j++) {
    //         J(i, j) = diff(ixy(i), x0[0]);
    //     }
    // }
}
#endif

TEST_F(eigen, 007) {
    Eigen::Transform<Symbol, 3, Eigen::Isometry> iso = Eigen::Transform<Symbol, 3, Eigen::Isometry>::Identity();
    Eigen::AngleAxis<Symbol> Rx(x0[0], Vector3(1, 0, 0));
    iso.translate(Vector3(x0[0], x0[1], x0[2]));
    iso.rotate(Rx);
    std::cout << iso.matrix() << std::endl;
    std::cout << iso.inverse().matrix() << std::endl;
    // iso.translate(Vector3(x0[0], x0[1], x0[2]));
}


}  // namespace
