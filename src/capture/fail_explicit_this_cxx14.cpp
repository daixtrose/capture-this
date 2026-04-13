// MUST NOT COMPILE under C++14 — [=, this] is C++17+ (P0409R2)

#include <functional>

struct Widget {
    int value = 42;

    std::function<int()> get_lambda() {
        int local = 10;
        return [=, this]() { return value + local; };
    }
};

int main() {
    Widget w;
    auto fn = w.get_lambda();
    return fn() != 52;
}
