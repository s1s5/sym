/**
 * Copyright 
 * @file test_002.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-26 19:20:53
 */
#include "sym/sym.hpp"

#include <iostream>

using namespace sym;

class MyFactory : public Factory {
 public:
    DynamicInput x0{"x0", 1};
    DynamicInput x1{"x1", 1};
    DynamicOutput y{"y", 2};

    void generate() {
        y[0] = x0[0] * x1[0] + x1[0];
        y[1] = diff(x0[0] + x1[0] * x0[0] * x0[0], x0[0]);
    }
};
OUTPUT_CXX_CODE_MAIN(MyFactory);

