// [*this] mutable — C++17
// The copy is const by default; `mutable` allows modification.

#include <boost/ut.hpp>
#include <functional>

struct C
{
    int i = 41;

    std::function<int()> make_mutable_lambda()
    {
        return [*this]() mutable
        {
            ++i;
            return i;
        };
    }
};

int main()
{
    using namespace boost::ut;

    "[*this] mutable increments the copy"_test = []
    {
        C c;
        auto fn = c.make_mutable_lambda();

        expect(42_i == fn()) << "first call: 41 + 1 = 42";
        expect(43_i == fn()) << "second call: 42 + 1 = 43";
        expect(41_i == c.i) << "original unchanged";
    };

    "[*this] without mutable — i += 10 has no effect"_test = []
    {
        struct S
        {
            int i = 42;
            std::function<int()> make()
            {
                return [*this]()
                { return i; };
            }
        };

        S s;
        auto fn = s.make();
        expect(42_i == fn());
        s.i += 10;
        expect(42_i == fn()) << "copy is independent";
    };
}
