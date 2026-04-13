// MUST NOT COMPILE — [i] cannot capture data member directly
// error: capture of non-variable 'C::i'

struct C {
    int i = 42;
    void process() {
        auto fn = [i]() { return i; };
    }
};

int main() {
    C c;
    c.process();
}
