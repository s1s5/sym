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
    ASSERT_NEAR(y[0]->eval(), 3, 1.0e-16);
}

TEST_F(four_arithmetic_operations, add_diff) {
    y[0] = x[0] + x[1];
    ASSERT_EQ(diff(y[0], x[0]), one());
    ASSERT_EQ(diff(y[0], x[1]), one());
    ASSERT_EQ(diff(y[0], x[2]), zero());
}

}  // namespace
