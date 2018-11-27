/**
 * Copyright Shogo Sawai
 * @file test_eigen_000.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-27 22:49:25
 */
// #include <iostream>
#include <Eigen/Eigen>
#include "sym/sym_eigen.hpp"


using namespace sym;

class MyFactory : public Factory {
 public:
    StaticInput p{"p", 3};
    DynamicInput xy{"xy", 2};
    DynamicOutput j{"j", 6};

    void generate() {
        Vector2 xy_i(
            cos(p[0]) * xy[0] - sin(p[0]) * xy[1] + p[1],
            sin(p[0]) * xy[0] + cos(p[0]) * xy[1] + p[2]);
        auto J = asMatrix<2, 3>(j);
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                J(i, j) = diff(xy_i(i), p[j]);
                // std::cout << "(" << i << ", " << j << ") : " << J(i, j) << std::endl;
            }
        }
    }
};
OUTPUT_CXX_CODE_MAIN(MyFactory);

