// [&i = i] — init-capture reference to data member (C++14+)
// Creates a reference bound to this->i without capturing this.

#include <boost/ut.hpp>
#include <functional>

struct C
{
    int i = 42;

    std::function<int()> make()
    {
        return [&i = i]()
        { return i; };
    }
};

int main()
{
    using namespace boost::ut;

    "[&i = i] binds a reference to the member"_test = []
    {
        C c;
        auto fn = c.make();

        expect(42_i == fn());
        c.i = 99;
        expect(99_i == fn()) << "reference follows the member";
    };

    "[i = i] copies the member value"_test = []
    {
        struct S
        {
            int i = 42;
            std::function<int()> make()
            {
                return [i = i]()
                { return i; };
            }
        };

        S s;
        auto fn = s.make();
        expect(42_i == fn());
        s.i = 99;
        expect(42_i == fn()) << "copy is independent";
    };
}
