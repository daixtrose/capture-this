// 03_init_capture.cpp — Init capture workaround (C++14 and later)
//
// Compiles with: -std=c++14, -std=c++17, -std=c++20  → OK
//                -std=c++11                            → ERROR
// Before C++17's [*this], this was the only way to capture by copy.

#include <functional>
#include <iostream>

struct Widget
{
    int value = 42;

    std::function<int()> get_lambda()
    {
        // C++14 init capture: copy *this into a new variable 'self'
        return [self = *this]()
        { return self.value; };
    }
};

int main()
{
    Widget w;
    auto fn = w.get_lambda();
    std::cout << "value via lambda: " << fn() << '\n';

    w.value = 99;
    std::cout << "after mutation:   " << fn() << '\n';
    // 'self' is a copy — still prints 42
}
