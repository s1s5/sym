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
    StaticInput x{"x", 2};
    DynamicInput w{"w", 2};
    StaticOutput y{"y", 2};
    DynamicOutput z{"z", 3};
    
    void generate() {
        y[0] = x[0] + x[1] * x[1];
        y[1] = sin(w[0] * w[1]);
        z[0] = 0.5 * x[0];
        z[1] = cos(w[0] * 0.5);
        z[2] = exp(w[0] * x[1]);

        // std::cout << y[0] << ", " << y[1] << std::endl;

        auto f = functions[0]->shared_from_this();
    }

};

// int main(int argc, char **argv) {
//     MyFactory f;
//     f.generate();
//     std::cout << f.digraph() << std::endl;
//     std::cout << f.wholeGraph() << std::endl;
//     std::cout << f.cxxCodePrinter("namespace_name", "ClassName") << std::endl;

//     auto x = _make_shared<sym::Variable>("x");
//     auto y = _make_shared<sym::Variable>("y");
    
//     auto z = x + y;
//     auto dz = z->diff(x);
//     dz->simplified();
//     std::cout << "x = " << x << std::endl;
//     std::cout << "y = " << y << std::endl;
//     std::cout << "z = " << z << std::endl;
//     std::cout << "dz/dx = " << dz << " : " << "is_one=" << is_one(dz) << ", is_constant=" << is_constant(dz) << std::endl;

//     sym::Repr r = sym::_repr("(", 3, "+", 4, ")");
    
//     return 0;
// }

OUTPUT_CXX_CODE_MAIN(MyFactory);
