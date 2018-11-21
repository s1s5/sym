/**
 * Copyright 2015- Co. Ltd. sizebook
 * @file test_000.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:14:53
 */
#include "sym/sym.hpp"

#include <iostream>

using namespace sym;

class MyFactory : public Factory {
 public:
    Input x{"x", 2};
    Output y{"y", 2};
    
    void generate() {
        y[0] = x[0] + x[1];
        y[1] = x[0] * x[1];

        std::cout << y[0] << ", " << y[1] << std::endl;
    }

};

int main(int argc, char **argv) {
    MyFactory f;
    f.generate();

    auto x = std::make_shared<sym::Variable>("x");
    auto y = std::make_shared<sym::Variable>("y");
    
    auto z = x + y;
    auto dz = z->diff(x);
    dz->simplified();
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "z = " << z << std::endl;
    std::cout << "dz/dx = " << dz << " : " << is_one(dz) << std::endl;

    
    return 0;
}
