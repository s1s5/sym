/**
 * Copyright 
 * @file test_001.cpp
 * @brief
 * @author Shogo Sawai
 * @date 2018-11-22 17:25:08
 */
#include "sym/sym.hpp"

#include <iostream>

using namespace sym;

class MyFactory : public Factory {
 public:
    StaticInput x0{"x0", 1};
    DynamicInput x1{"x1", 1};
    StaticOutput y{"y", 2};

    void generate() {
        y[0] = x0[0] * x1[0];
        y[1] = diff(y[0], x0[0]);
    }
};
OUTPUT_CXX_CODE_MAIN(MyFactory);

// $ ./example/example_001  -n my_namespace -c MyClass
// namespace my_namespace {
// template<class ProbeScalar = double, class IntermediateScalar = double>
// class MyClass {
//  public:
//     MyClass(ProbeScalar *x0_, ProbeScalar *y_) : x0(x0_), y(y_) {
//         refresh();
//     }
// 
//     void refresh() {
//         y[1] = x1[0];
//         _i[0] = x0[0];
//     }
// 
//     void operator()(ProbeScalar *x1) {
//         y[0] = (_i[0]*x1[0]);
//     }
// 
//     ProbeScalar *x0;
//     ProbeScalar *y;
//     IntermediateScalar _i[1];
// };
// }  // namespace my_namespace
