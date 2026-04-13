// Deducing this — C++23 (P0847R7)
// Recursive lambda without std::function, using explicit object parameter.

#include <boost/ut.hpp>

int main() {
    using namespace boost::ut;

    "deducing this: recursive lambda"_test = [] {
        auto fib = [](this auto self, int n) -> int {
            return n <= 1 ? n : self(n - 1) + self(n - 2);
        };

        expect(0_i == fib(0));
        expect(1_i == fib(1));
        expect(1_i == fib(2));
        expect(2_i == fib(3));
        expect(5_i == fib(5));
        expect(8_i == fib(6));
    };

    "deducing this with [&i = i] in a class"_test = [] {
        struct C {
            int i = 0;
            void process() {
                auto fib = [&i = i](this auto self, int n) -> int {
                    ++i;
                    return n <= 1 ? n : self(n - 1) + self(n - 2);
                };
                auto r = fib(3);
                boost::ut::expect(boost::ut::_i(5) == i) << "5 calls to fib";
                boost::ut::expect(boost::ut::_i(2) == r) << "fib(3) = 2";
            }
        };
        C c;
        c.process();
    };
}
