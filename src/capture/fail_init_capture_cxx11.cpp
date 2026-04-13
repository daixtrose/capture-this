// MUST NOT COMPILE under C++11 — init captures are C++14+
// [self = *this] is a generalized lambda capture (P0018R0).

#include <functional>

struct Widget {
    int value = 42;

    std::function<int()> get_lambda() {
        return [self = *this]() { return self.value; };
    }
};

int main() {
    Widget w;
    auto fn = w.get_lambda();
    return fn() != 42;
}
