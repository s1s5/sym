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
    ASSERT_EQ(diff(y[0], x[0])->id(), one()->id());
    ASSERT_EQ(diff(y[0], x[1])->id(), one()->id());
    ASSERT_EQ(diff(y[0], x[2])->id(), zero()->id());
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
    ASSERT_EQ(diff(y[0], x[0])->id(), one()->id());
    ASSERT_EQ(diff(y[0], x[1])->id(), (- one())->id());
    ASSERT_EQ(diff(y[0], x[2])->id(), zero()->id());
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
    ASSERT_EQ(diff(y[0], x[0])->id(), x[1]->id());
    ASSERT_EQ(diff(y[0], x[1])->id(), x[0]->id());
    ASSERT_EQ(diff(y[0], x[2])->id(), zero()->id());
    ASSERT_EQ(diff(y[1], x[0])->id(), (x[0] + x[0])->id());
}

TEST_F(four_arithmetic_operations, div_eval) {
    x.assign({1, 2, 3});
    y[0] = x[0] / x[1];
    ASSERT_NEAR(y[0]->eval(), 0.5, 1.0e-16);
}

TEST_F(four_arithmetic_operations, div_diff) {
    y[0] = x[0] / x[1];
    ASSERT_EQ(diff(y[0], x[0])->id(), (1 / x[1])->id());
    ASSERT_EQ(diff(y[0], x[1])->id(), (- (x[0] / (x[1] * x[1])))->id());
    ASSERT_EQ(diff(y[0], x[2])->id(), zero()->id());
}

}  // namespace
