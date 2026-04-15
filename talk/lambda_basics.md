---
layout: cover
background: /images/DSC_0648.JPG
---

# Recap: C++ $\lambda$ Basics

---

# What are Lambdas?

Lambdas are `syntactic sugar` for **function objects**:

````md magic-move {lines: true}
```cpp
#include <iostream> // C++11

struct C {
    int operator()() {
        return 42;
    } 
};

int main()
{
    C c;
    std::cout << c() << "\n";
}
```

```cpp
#include <iostream>

auto c = []() {
    return 42;
};

int main()
{
    std::cout << c() << "\n";
}
```

```cpp
#include <iostream>

int main()
{
    auto c = []() {
      return 42;
    };
    
    std::cout << c() << "\n";
}
```
````

<div v-show="$clicks === 0">
<p>Output:</p>
<pre><code>42</code></pre>
</div>

<div v-show="$clicks === 1">
<p>Output is still:</p>
<pre><code>42</code></pre>
</div>

<div v-show="$clicks >= 2">
<p>Moving lambda into a function still yields:</p>
<pre><code>42</code></pre>
</div>



---

# Lambdas Capturing by Value 

```cpp
// highlight-word:[=]
#include <print>

int main() {
    int i = 41;
    // capture by value
    auto get_41 = [=] () { return i; };
    ++i;
    // capture by value again, drop () which is not necessary for empty arg list
    auto get_42 = [=] { return i; };
    std::println("first capture: {}", get_41());
    std::println("second capture: {}", get_42());
}
```

Output:

```
first capture: 41 
second capture: 42
```

---

# Lambdas Capturing by Reference 

```cppg
// highlight-word:[&]
#include <print>

int main() {
    int i = 41;
    // capture by value
    auto get_41 = [&] { return i; };
    ++i;
    // capture by value again
    auto get_42 = [&] { return i; };
    std::println("first capture: {}", get_41());
    std::println("second capture: {}", get_42());
}
```

Output:

``` {1}
// highlight-word:// <-- Ouch!
first capture: 42 // <-- Ouch!
second capture: 42
```


---

# Lambdas With Capturings And Arguments

```cpp
// highlight-word:(int & refI)
#include <print>

int main() {
    int i = 41;
    // capture by value
    auto print_1 = [=] (int & refI) { std::println("i: {0}, refI: {1}", i, refI); };
    ++i;
    // capture by value again
    auto print_2 = [=] (int & refI) { std::println("i: {0}, refI: {1}", i, refI); };

    print_1(i);
    print_2(i);
}
```

Output:

```
i: 41, refI: 42
i: 42, refI: 42
```

---

# Lambdas Individual Capturing  

```cpp
// highlight-word:&d
#include <print>

int main() {
    int i = 41;
    double d = 41.0;
    auto print_1 = [i, &d] { std::println("i: {0}, d: {1:.2f}", i, d); };
    ++i;
    d += 1.0; 
    auto print_2 = [i, &d] { std::println("i: {0}, d: {1:.2f}", i, d); };
    print_1();
    print_2();
}
```

Output:

``` 
i: 41, d: 42.00
i: 42, d: 42.00
```

---

# Lambdas Capturing to New Name  

```cpp
// highlight-word:x=u
#include <print>

int main() {
    int u = 42;
    auto print = [x=u] () { std::println("x: {0}", x); };
    print();
}
```

Output:

``` 
x: 42
```
