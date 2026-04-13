// MUST NOT COMPILE — [] cannot access data member `i`
// `this` was not captured for this lambda function.

struct C {
    int i = 42;
    void process() {
        auto print = []() { return i; };
    }
};

int main() {
    C c;
    c.process();
}
