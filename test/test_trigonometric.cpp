/**
 * Copyright 
 * @file test_trigonometric.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 11:55:42
 */
#include "cpput.hpp"

#include "sym/sym.hpp"

namespace {

using namespace sym;

struct trigonometric : public Factory {
    StaticInput x{"x", 3};
    StaticOutput y{"y", 3};
};

TEST_F(trigonometric, sin_eval) {
    x.assign({1, 2, 3});
    y[0] = sin(x[0]);
    y[1] = sin(x[1]);
    y[2] = sin(x[2]);
    ASSERT_NEAR(y[0]->eval(), std::sin(1), 1.0e-16);
    ASSERT_NEAR(y[1]->eval(), std::sin(2), 1.0e-16);
    ASSERT_NEAR(y[2]->eval(), std::sin(3), 1.0e-16);
}

TEST_F(trigonometric, sin_diff) {
    y[0] = sin(x[0]);
    y[1] = sin(x[0] * x[1]);
    ASSERT_EQ(diff(y[0], x[0])->id(), cos(x[0])->id());
    ASSERT_EQ(diff(y[0], x[1])->id(), zero()->id());
}

TEST_F(trigonometric, cos_eval) {
    x.assign({1, 2, 3});
    y[0] = cos(x[0]);
    y[1] = cos(x[1]);
    y[2] = cos(x[2]);
    ASSERT_NEAR(y[0]->eval(), std::cos(1), 1.0e-16);
    ASSERT_NEAR(y[1]->eval(), std::cos(2), 1.0e-16);
    ASSERT_NEAR(y[2]->eval(), std::cos(3), 1.0e-16);
}

TEST_F(trigonometric, cos_diff) {
    y[0] = cos(x[0]);
    y[1] = cos(x[0] * x[1]);
    ASSERT_EQ(diff(y[0], x[0])->id(), (- sin(x[0]))->id());
    ASSERT_EQ(diff(y[0], x[1])->id(), zero()->id());
}


}  // namespace

