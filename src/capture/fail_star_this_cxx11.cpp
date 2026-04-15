// MUST NOT COMPILE under C++11 — [*this] is C++17+ (P0018R3)

#include <functional>

struct Widget
{
    int value = 42;

    std::function<int()> get_lambda()
    {
        return [*this]()
        { return value; };
    }
};

int main()
{
    Widget w;
    auto fn = w.get_lambda();
    return fn() != 42;
}
