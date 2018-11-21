/**
 * Copyright 2015- Co. Ltd. sizebook
 * @file test_000.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-21 14:14:53
 */
#include "sym/sym.hpp"

#include <iostream>

// using namespace sym;

class MyFactory : public sym::Factory {
 public:
    
};

int main(int argc, char **argv) {
    MyFactory f;

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
