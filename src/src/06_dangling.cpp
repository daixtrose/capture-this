// 06_dangling.cpp — The dangling-this problem that motivates [*this]
//
// Compiles with: -std=c++11, -std=c++14, -std=c++17, -std=c++20  → OK
// RUNTIME BUG: demonstrates undefined behavior when a lambda outlives
// the object whose 'this' pointer it captured.

#include <functional>
#include <iostream>

struct Widget
{
    int value = 42;

    std::function<int()> get_lambda()
    {
        // Captures 'this' pointer — dangerous if Widget is destroyed
        return [this]()
        { return value; };
    }
};

int main()
{
    std::function<int()> fn;

    {
        Widget* w = new Widget();
        fn = w->get_lambda();
        std::cout << "while alive: " << fn() << '\n'; // OK: 42
        delete w;
    }
    // w is destroyed here — fn now holds a dangling pointer

    // UNDEFINED BEHAVIOR — may crash, return garbage, or "work"
    // std::cout << "after destroy: " << fn() << '\n';

    std::cout << "// Uncomment the line above to see undefined behavior!\n";
    std::cout << "// This is exactly why C++17 added [*this].\n";
}
