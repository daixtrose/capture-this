// 02_star_this.cpp — Capturing `*this` by copy (C++17 and later)
//
// Compiles with: -std=c++17, -std=c++20     → OK
//                -std=c++11, -std=c++14      → ERROR
// Demonstrates the key C++17 addition: capturing the object by value.

#include <functional>
#include <iostream>

struct Widget
{
    int value = 42;

    std::function<int()> get_lambda()
    {
        // [*this] captures a COPY of the entire object
        return [*this]()
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
    // The lambda's copy is independent — still prints 42
}
