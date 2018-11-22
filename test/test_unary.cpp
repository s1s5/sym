/**
 * Copyright 
 * @file test_unary.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 11:41:14
 */
#include "cpput.hpp"

#include "sym/sym.hpp"

namespace {

using namespace sym;

struct unary : public Factory {
    StaticInput x{"x", 3};
    StaticOutput y{"y", 3};
};

TEST_F(unary, neg_eval) {
    x.assign({1, 2, 3});
    y[0] = - x[0];
    ASSERT_NEAR(y[0]->eval(), -1, 1.0e-16);
}

TEST_F(unary, neg_diff) {
    y[0] = - x[0];
    ASSERT_EQ(diff(y[0], x[0])->id(), (- one())->id());
    ASSERT_EQ(diff(y[0], x[1])->id(), zero()->id());
}

TEST_F(unary, sqrt_eval) {
    x.assign({1, 2, 3});
    y[0] = sqrt(x[0]);
    y[1] = sqrt(x[1]);
    y[2] = sqrt(x[2]);
    ASSERT_NEAR(y[0]->eval(), std::sqrt(1), 1.0e-16);
    ASSERT_NEAR(y[1]->eval(), std::sqrt(2), 1.0e-16);
    ASSERT_NEAR(y[2]->eval(), std::sqrt(3), 1.0e-16);
}

TEST_F(unary, sqrt_diff) {
    x.assign({2, 2, 3});
    y[0] = sqrt(x[0]);
    y[1] = sqrt(x[0] * x[1]);
    ASSERT_EQ(diff(y[0], x[0])->id(), (1 / (2 * sqrt(x[0])))->id());
    ASSERT_NEAR(diff(y[0], x[0])->eval(), (1.0 / (2 * std::sqrt(2))), 1.0e-8);
    ASSERT_EQ(diff(y[0], x[1])->id(), zero()->id());
}

TEST_F(unary, exp_eval) {
    x.assign({1, 2, 3});
    y[0] = exp(x[0]);
    y[1] = exp(x[1]);
    y[2] = exp(x[2]);
    ASSERT_NEAR(y[0]->eval(), std::exp(1.0), 1.0e-16);
    ASSERT_NEAR(y[1]->eval(), std::exp(2.0), 1.0e-16);
    ASSERT_NEAR(y[2]->eval(), std::exp(3.0), 1.0e-16);
}

TEST_F(unary, exp_diff) {
    x.assign({2, 3, 4});
    y[0] = exp(x[0]);
    y[1] = exp(x[0] * x[1]);
    ASSERT_EQ(diff(y[0], x[0])->id(), exp(x[0])->id());
    ASSERT_EQ(diff(y[0], x[0])->eval(), std::exp(2));
    ASSERT_EQ(diff(y[0], x[1])->id(), zero()->id());
}

TEST_F(unary, log_eval) {
    x.assign({1, 2, 3});
    y[0] = log(x[0]);
    y[1] = log(x[1]);
    y[2] = log(x[2]);
    ASSERT_NEAR(y[0]->eval(), std::log(1), 1.0e-16);
    ASSERT_NEAR(y[1]->eval(), std::log(2), 1.0e-16);
    ASSERT_NEAR(y[2]->eval(), std::log(3), 1.0e-16);
}

TEST_F(unary, log_diff) {
    x.assign({2, 3, 4});
    y[0] = log(x[0]);
    y[1] = log(x[0] * x[1]);
    ASSERT_EQ(diff(y[0], x[0])->id(), (1 / x[0])->id());
    ASSERT_EQ(diff(y[0], x[1])->id(), zero()->id());
}


}  // namespace
