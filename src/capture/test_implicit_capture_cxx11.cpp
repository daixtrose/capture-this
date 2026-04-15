// [=] and [&] implicit this capture — C++11
// Both capture `this` as a pointer. [=] does NOT copy the object.

#include <boost/ut.hpp>
#include <functional>

struct C
{
    int i = 42;

    std::function<int()> capture_by_eq()
    {
        return [=]()
        { return i; };
    }
    std::function<int()> capture_by_ref()
    {
        return [&]()
        { return i; };
    }
};

int main()
{
    using namespace boost::ut;

    "[=] implicitly captures this as pointer"_test = []
    {
        C c;
        auto fn = c.capture_by_eq();
        expect(42_i == fn());

        c.i = 99;
        expect(99_i == fn()) << "[=] sees mutation — it's a pointer, not a copy!";
    };

    "[&] implicitly captures this as pointer"_test = []
    {
        C c;
        auto fn = c.capture_by_ref();
        expect(42_i == fn());

        c.i = 99;
        expect(99_i == fn());
    };

    "[=] WTF: ++i between calls is visible"_test = []
    {
        struct S
        {
            int i = 42;
            std::function<int()> make()
            {
                return [=]()
                { return i; };
            }
        };

        S s;
        auto fn = s.make();
        expect(42_i == fn());
        ++s.i;
        expect(43_i == fn()) << "WTF: [=] looks like by-value but i mutated";
    };
}
