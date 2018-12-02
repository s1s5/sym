/**
 * Copyright 
 * @file test_four_arithmetic_operations.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 10:23:36
 */
#include "cpput.hpp"

#include "sym/sym.hpp"

namespace {

using namespace sym;

struct four_arithmetic_operations : public Factory {
    StaticInput x{"x", 3};
    StaticOutput y{"y", 3};
};

TEST_F(four_arithmetic_operations, add_eval) {
    x.assign({1, 2, 3});
    y[0] = x[0] + x[1];
    y[1] = x[0] + x[1] + x[2];
    ASSERT_NEAR(y[0]->eval(), 3, 1.0e-16);
    ASSERT_NEAR(y[1]->eval(), 6, 1.0e-16);
}

TEST_F(four_arithmetic_operations, add_diff) {
    y[0] = x[0] + x[1];
    ASSERT_EQ(diff(y[0], x[0]), one());
    ASSERT_EQ(diff(y[0], x[1]), one());
    ASSERT_EQ(diff(y[0], x[2]), zero());
}

TEST_F(four_arithmetic_operations, add_simplified) {
    y[0] = (x[0] + 1);
    // std::cout << y[0].repr() << std::endl;
    ASSERT_EQ(y[0].repr(), "(1.000000+x[0])");
    y[0] = (x[0] + 1) + (x[1] + 1);
    ASSERT_EQ(y[0].repr(), "((2.000000+x[0])+x[1])");
}

TEST_F(four_arithmetic_operations, sub_eval) {
    x.assign({1, 2, 3});
    y[0] = x[0] - x[1];
    y[1] = x[0] - x[1] - x[2];
    ASSERT_NEAR(y[0]->eval(), -1, 1.0e-16);
    ASSERT_NEAR(y[1]->eval(), -4, 1.0e-16);
}

TEST_F(four_arithmetic_operations, sub_diff) {
    y[0] = x[0] - x[1];
    ASSERT_EQ(diff(y[0], x[0]), one());
    ASSERT_EQ(diff(y[0], x[1]), (- one()));
    ASSERT_EQ(diff(y[0], x[2]), zero());
}

TEST_F(four_arithmetic_operations, mul_eval) {
    x.assign({1, 2, 3});
    y[0] = x[0] * x[1];
    y[1] = x[0] * x[1] * x[2];
    ASSERT_NEAR(y[0]->eval(), 2, 1.0e-16);
    ASSERT_NEAR(y[1]->eval(), 6, 1.0e-16);
}

TEST_F(four_arithmetic_operations, mul_diff) {
    y[0] = x[0] * x[1];
    y[1] = x[0] * x[0];
    ASSERT_EQ(diff(y[0], x[0]), x[1]);
    ASSERT_EQ(diff(y[0], x[1]), x[0]);
    ASSERT_EQ(diff(y[0], x[2]), zero());
    ASSERT_EQ(diff(y[1], x[0]), (x[0] + x[0]));
}

TEST_F(four_arithmetic_operations, mul_simplified) {
    y[0] = 1 * x[0];
    ASSERT_EQ(y[0].repr(), "x[0]");
    y[0] = 1 * x[0] * 3;
    ASSERT_EQ(y[0].repr(), "(3.000000*x[0])");
    y[0] = 2 * x[0] * 2;
    ASSERT_EQ(y[0].repr(), "(4.000000*x[0])");
    // y[0] = 2 * x[0] * 2 * x[0];
    // std::cout << y[0].repr() << std::endl;
}

TEST_F(four_arithmetic_operations, div_eval) {
    x.assign({1, 2, 3});
    y[0] = x[0] / x[1];
    ASSERT_NEAR(y[0]->eval(), 0.5, 1.0e-16);
}

TEST_F(four_arithmetic_operations, div_diff) {
    y[0] = x[0] / x[1];
    ASSERT_EQ(diff(y[0], x[0]), (1 / x[1]));
    // TODO
    // std::cout << diff(y[0], x[1]) << std::endl;
    // ASSERT_EQ(diff(y[0], x[1]), (- (x[0] / (x[1] * x[1]))));
    ASSERT_EQ(diff(y[0], x[2]), zero());
}

}  // namespace
