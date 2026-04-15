// [self=*this] init capture — C++14 workaround for copying *this

#include <boost/ut.hpp>
#include <functional>

struct Widget
{
    int value = 42;

    std::function<int()> get_lambda()
    {
        return [self = *this]()
        { return self.value; };
    }
};

int main()
{
    using namespace boost::ut;

    "[self=*this] captures a copy of the object"_test = []
    {
        Widget w;
        auto fn = w.get_lambda();
        expect(42_i == fn());

        w.value = 99;
        expect(42_i == fn()) << "copy is independent — still 42";
    };
}
