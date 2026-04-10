// 04_implicit_this.cpp — Implicit capture of `this` via [=] and [&]
//
// Compiles with: -std=c++11, -std=c++14, -std=c++17, -std=c++20  → OK
// But C++20 emits a DEPRECATION WARNING for [=] capturing this.
// [&] still implicitly captures this without warning in all standards.

#include <functional>
#include <iostream>

struct Widget
{
    int value = 42;

    std::function<int()> lambda_by_copy()
    {
        // [=] implicitly captures 'this' (pointer, NOT a copy of *this!)
        // C++20: deprecated — "implicit capture of 'this' via '[=]' is deprecated"
        return [=]()
        { return value; };
    }

    std::function<int()> lambda_by_ref()
    {
        // [&] also implicitly captures 'this' — no deprecation in any standard
        return [&]()
        { return value; };
    }
};

int main()
{
    Widget w;

    auto fn_copy = w.lambda_by_copy();
    auto fn_ref = w.lambda_by_ref();

    std::cout << "[=] lambda: " << fn_copy() << '\n';
    std::cout << "[&] lambda: " << fn_ref() << '\n';

    w.value = 99;
    std::cout << "[=] after:  " << fn_copy() << '\n'; // also 99! (pointer, not copy)
    std::cout << "[&] after:  " << fn_ref() << '\n';
}
