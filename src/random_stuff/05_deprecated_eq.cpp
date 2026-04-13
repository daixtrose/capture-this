// 05_deprecated_eq.cpp — [=, this] and [=, *this] (C++17 and later)
//
// Compiles with: -std=c++17, -std=c++20  → OK (no warnings)
//                -std=c++11, -std=c++14  → ERROR
// These explicit forms were added in C++17 precisely to avoid the
// implicit-this-via-[=] trap that C++20 deprecates.

#include <functional>
#include <iostream>

struct Widget
{
    int value = 42;
    int extra = 10;

    std::function<int()> explicit_this_pointer()
    {
        int local = extra;
        // [=, this] — capture locals by copy, 'this' explicitly as pointer
        return [=, this]()
        { return value + local; };
    }

    std::function<int()> explicit_this_copy()
    {
        int local = extra;
        // [=, *this] — capture locals by copy AND copy the whole object
        return [=, *this]()
        { return value + local; };
    }
};

int main()
{
    Widget w;

    auto fn_ptr = w.explicit_this_pointer();
    auto fn_cpy = w.explicit_this_copy();

    std::cout << "[=, this]:  " << fn_ptr() << '\n'; // 52
    std::cout << "[=, *this]: " << fn_cpy() << '\n'; // 52

    w.value = 99;
    std::cout << "[=, this]  after mutation: " << fn_ptr() << '\n'; // 109 (pointer)
    std::cout << "[=, *this] after mutation: " << fn_cpy() << '\n'; // 52  (copy)
}
