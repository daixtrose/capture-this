// 01_this_pointer.cpp — Capturing `this` as a pointer (C++11 and later)
//
// Compiles with: -std=c++11, -std=c++14, -std=c++17, -std=c++20
// All standards: OK

#include <functional>
#include <iostream>

struct Widget
{
    int value = 42;

    std::function<int()> get_lambda()
    {
        // [this] captures the 'this' pointer — not a copy of the object
        return [this]()
        { return value; };
    }
};

int main()
{
    Widget w;
    auto fn = w.get_lambda();
    std::cout << "value via lambda: " << fn() << '\n';

    w.value = 99;
    std::cout << "after mutation:   " << fn() << '\n';
    // Both calls access the same object — the lambda holds a pointer
}
