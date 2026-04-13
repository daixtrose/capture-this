// [this] pointer capture — C++11 and later
// The lambda holds a pointer to the object; mutations are visible.

#include <boost/ut.hpp>
#include <functional>

struct Widget {
    int value = 42;

    std::function<int()> get_lambda() {
        return [this]() { return value; };
    }
};

int main() {
    using namespace boost::ut;

    "[this] captures a pointer to the object"_test = [] {
        Widget w;
        auto fn = w.get_lambda();
        expect(42_i == fn());

        w.value = 99;
        expect(99_i == fn()) << "mutation visible through pointer";
    };

    "[this] and [=] behave the same for members"_test = [] {
        struct C {
            int i = 42;
            std::function<int()> via_this() { return [this]() { return i; }; }
            std::function<int()> via_eq()   { return [=]()    { return i; }; }
        };

        C c;
        auto fn_this = c.via_this();
        auto fn_eq   = c.via_eq();

        c.i = 99;
        expect(fn_this() == fn_eq()) << "[this] and [=] both capture this pointer";
        expect(99_i == fn_this());
    };
}
