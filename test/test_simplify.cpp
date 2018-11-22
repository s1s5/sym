/**
 * Copyright 
 * @file test_simplify.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 15:51:20
 */
#include "cpput.hpp"

#include "sym/sym.hpp"

namespace {

using namespace sym;

struct simplify : public Factory {
    StaticInput x{"x", 3};
    StaticOutput y{"y", 3};
};

TEST_F(simplify, add_sub) {
    ASSERT_EQ(x[0] - x[0], zero());
    ASSERT_EQ(- x[0] + x[0], zero());
    ASSERT_EQ((- x[0]) + x[0], zero());
    ASSERT_EQ(cos(x[0]) - cos(x[0]), zero());
    ASSERT_EQ((- exp(x[0])) + exp(x[0]), zero());
    ASSERT_EQ((1 - sin(x[0])) + sin(x[0]), one());
    ASSERT_EQ(1 * (1 - sin(x[0])) + sin(x[0]), one());
    ASSERT_EQ(1 * (1 - sin(x[0])) + 1 * sin(x[0]), one());
    ASSERT_EQ(0 * (1 - sin(x[0])) + 0 * sin(x[0]), zero());
    // std::cout << cos(x[0]) * (1 - sin(x[0])) + (-1 + sin(x[0])) * cos(x[0]) << std::endl;
    // ASSERT_EQ(cos(x[0]) * (1 - sin(x[0])) + (1 + sin(x[0])) * cos(x[0]), zero());
}

}  // namespace
