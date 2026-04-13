// [*this] copy capture — C++17

#include <boost/ut.hpp>
#include <functional>

struct Widget {
    int value = 42;

    std::function<int()> get_lambda() {
        return [*this]() { return value; };
    }
};

int main() {
    using namespace boost::ut;

    "[*this] captures a copy of the entire object"_test = [] {
        Widget w;
        auto fn = w.get_lambda();
        expect(42_i == fn());

        w.value = 99;
        expect(42_i == fn()) << "copy is independent";
    };

    "[*this] copy vs [this] pointer"_test = [] {
        struct C {
            int i = 42;
            std::function<int()> by_ptr()  { return [this]()  { return i; }; }
            std::function<int()> by_copy() { return [*this]() { return i; }; }
        };

        C c;
        auto ptr  = c.by_ptr();
        auto copy = c.by_copy();

        c.i = 99;
        expect(99_i == ptr())  << "[this] sees mutation";
        expect(42_i == copy()) << "[*this] is independent";
    };
}
