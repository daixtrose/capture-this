// Dangling this — demonstrates why [*this] exists.
// We can only test the safe path; the UB path is commented out.

#include <boost/ut.hpp>
#include <functional>

struct Widget
{
    int value = 42;

    std::function<int()> get_lambda_ptr()
    {
        return [this]()
        { return value; };
    }
    std::function<int()> get_lambda_copy()
    {
        return [*this]()
        { return value; };
    }
};

int main()
{
    using namespace boost::ut;

    "[*this] survives object destruction"_test = []
    {
        std::function<int()> fn;
        {
            Widget w;
            fn = w.get_lambda_copy();
        }
        // w is destroyed, but fn holds a copy
        expect(42_i == fn()) << "[*this] copy is safe";
    };

    // NOTE: The [this] pointer version would be undefined behavior:
    //   Widget* w = new Widget();
    //   auto fn = w->get_lambda_ptr();
    //   delete w;
    //   fn();  // UB: dangling pointer
    // We intentionally do NOT test this — it's the teaching moment.
}
