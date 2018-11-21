# c++ sym library


# example

``` c++
// Generator code
class Func : public Factory {
 public:
    DynamicInput x{"x", 3};
    StaticOutput y{"y", 2};

    void generate() {
        y[0] = x[0] * x[1] + x[2];
        y[1] = diff(y[0], x[0]);
    }
};
OUTPUT_CXX_CODE_MAIN(Func, ns, cls);

// usage
ns::cls f(y);
f(x);
std::cout << y[0] << ", " << y[1] << std::endl;
```

``` c++
// Generator code
class Func : public Factory {
 public:
    StaticInput x0{"x0", 1};
    DynamicInput x1{"x1", 1};
    StaticOutput y{"y", 2};

    void generate() {
        y[0] = x0[0] * x1[0];
        y[1] = diff(y[0], x0[0]);
    }
};
OUTPUT_CXX_CODE_MAIN(Func, ns, cls);

// usage
ns::cls f(x0, y);
f(x1);
std::cout << y[0] << ", " << y[1] << std::endl;
f.refresh();  // recalculate with pointer that is set in constructor
f(x1_new);
```
