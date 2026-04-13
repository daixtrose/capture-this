// [=, this] and [=, *this] — C++17
// Explicit this capture alongside default by-value capture.

#include <boost/ut.hpp>
#include <functional>

struct Widget {
    int value = 42;
    int extra = 10;

    std::function<int()> explicit_this_pointer() {
        int local = extra;
        return [=, this]() { return value + local; };
    }

    std::function<int()> explicit_this_copy() {
        int local = extra;
        return [=, *this]() { return value + local; };
    }
};

int main() {
    using namespace boost::ut;

    "[=, this] — pointer + locals by value"_test = [] {
        Widget w;
        auto fn = w.explicit_this_pointer();
        expect(52_i == fn());

        w.value = 99;
        expect(109_i == fn()) << "pointer: sees value mutation";
    };

    "[=, *this] — copy + locals by value"_test = [] {
        Widget w;
        auto fn = w.explicit_this_copy();
        expect(52_i == fn());

        w.value = 99;
        expect(52_i == fn()) << "copy: independent of original";
    };

    "[*this] alone does not capture locals"_test = [] {
        struct C {
            int i = 42;
            void check() {
                int x = 99;
                auto with_star_this = [*this]() { return i; };
                auto with_eq_star   = [=, *this]() { return i + x; };

                expect(42_i == with_star_this());
                expect(141_i == with_eq_star()) << "[=, *this] captures both";
            }
        };
        C{}.check();
    };
}
