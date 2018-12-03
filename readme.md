# c++ sym library


# example

``` c++
// Generator code
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

// generated code
// $ ./example/example_000  -n my_namespace -c MyClass
namespace my_namespace {
template<class ProbeScalar = double, class IntermediateScalar = double>
class MyClass {
 public:
    MyClass(ProbeScalar *y_) : y(y_) {
        refresh();
    }

    void refresh() {
        y[1] = x[1];
    }

    void operator()(ProbeScalar *x) {
        IntermediateScalar _t3 = (x[0]*x[1]);
        y[0] = (_t3+x[2]);
    }

    ProbeScalar *y;
};
}  // namespace my_namespace

// usage
double x[3] = {1, 2, 3}, y[2];
my_namespace::MyClass f(y);
f(x);
std::cout << y[0] << ", " << y[1] << std::endl;
```

``` c++
// Generator code
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

// generated code
// $ ./example/example_001  -n my_namespace -c MyClass
namespace my_namespace {
template<class ProbeScalar = double, class IntermediateScalar = double>
class MyClass {
 public:
    MyClass(ProbeScalar *x0_, ProbeScalar *y_) : x0(x0_), y(y_) {
        refresh();
    }

    void refresh() {
        y[1] = x1[0];
        _i[0] = x0[0];
    }

    void operator()(ProbeScalar *x1) {
        y[0] = (_i[0]*x1[0]);
    }

    ProbeScalar *x0;
    ProbeScalar *y;
    IntermediateScalar _i[1];
};
}  // namespace my_namespace

// usage
double x0[1] = {1}, x1[1] = {2}, y[2];
my_namespace::MyClass f(x0, y);
f(x1);
std::cout << y[0] << ", " << y[1] << std::endl;
x0[0] = 3;
f.refresh();  // recalculate with pointer that is set in constructor
f(x1_new);
std::cout << y[0] << ", " << y[1] << std::endl;
```

# todo
- [ ] implement expand/factor
- [ ] optimize calculation graph
- [ ] multiple generator single source
- [ ] generic test
- [ ] add const for input variables
- [ ] bug fix eigen intermediate/laxy expression(auto x = y * z; cout << x << endl;), add test
- [ ] support neon
- [ ] more fine grained template class
- [ ] share intermediates in multiple generator
- [ ] more eigen aliases(AngleAxis ...)
- [ ] implement subs
