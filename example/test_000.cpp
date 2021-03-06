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
    DynamicInput x{"x", 3};
    StaticOutput y{"y", 2};

    void generate() {
        y[0] = x[0] * x[1] + x[2];
        y[1] = diff(y[0], x[0]);
    }
};

OUTPUT_CXX_CODE_MAIN(MyFactory);

// $ ./example/example_000  -n my_namespace -c MyClass
// namespace my_namespace {
// template<class ProbeScalar = double, class IntermediateScalar = double>
// class MyClass {
//  public:
//     MyClass(ProbeScalar *y_) : y(y_) {
//         refresh();
//     }
// 
//     void refresh() {
//         y[1] = x[1];
//     }
// 
//     void operator()(ProbeScalar *x) {
//         IntermediateScalar _t3 = (x[0]*x[1]);
//         y[0] = (_t3+x[2]);
//     }
// 
//     ProbeScalar *y;
// };
// }  // namespace my_namespace
