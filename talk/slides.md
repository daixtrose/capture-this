---
theme: ./daixtrose-basic
title: "Capture *this — The Evolution of C++ Class Member Lambda Expressions"
info: |
  ## Capture *this in Lambda Expressions
  A talk about the evolution of capturing `this` in C++ lambdas.

  C++ User Group Aachen
fonts:
  sans: Atkinson Hyperlegible Next
  mono: Intel One Mono
  local: Atkinson Hyperlegible Next,Intel One Mono
  provider: none
drawings:
  persist: false
transition: fade-out
colorSchema: both
toc:
  maxDepth: 1
  minDepth: 1
layout: cover
background: /images/DSC_0719_DxO.jpg
---

# Capture `*this`

> ## The Evolution of C++ Class Member Lambda Expressions

<div class="abs-br m-6 text-sm opacity-50">
  C++ User Group Aachen
</div>

<!--
Opening slide. Welcome the audience, introduce yourself, show QR codes.
-->

---
layout: two-cols
background: /images/IMG_7352.JPEG
transition: fade-out
---

# What This Talk Covers

How capturing `this` in lambdas evolved across C++ standards

- C++11 — `[this]` captures a pointer
- C++14 — init-capture workaround `[self = *this]`
- C++17 — `[*this]` capture by copy
- C++20 — `[=]` implicit `this` deprecated
- C++23 — `this`? deducing `this`! 
- C++26 — `this` is it. 
  <br>What are deprecations anyway?

<!--
Give a brief roadmap of what we'll cover.
-->

---

# Recap: What are Lambdas?

Lambdas are `syntactic sugar` for **function objects**:

````md magic-move {lines: true}
```cpp
#include <iostream>

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

# Lambdas Binding by Value 

```cpp
// highlight-word:[=]
#include <print>

int main() {
    int i = 41;
    // bind by value
    auto get_41 = [=] () { return i; };
    ++i;
    // bind by value again, drop () which is not necessary for empty arg list
    auto get_42 = [=] { return i; };
    std::println("first bind: {}", get_41());
    std::println("second bind: {}", get_42());
}
```

Output:

```
first bind: 41 
second bind: 42
```

---

# Lambdas Binding by Reference 

```cpp
// highlight-word:[&]
#include <print>

int main() {
    int i = 41;
    // bind by value
    auto get_41 = [&] { return i; };
    ++i;
    // bind by value again
    auto get_42 = [&] { return i; };
    std::println("first bind: {}", get_41());
    std::println("second bind: {}", get_42());
}
```

Output:

``` {1}
// highlight-word:// <-- Ouch!
first bind: 42 // <-- Ouch!
second bind: 42
```


---

# Lambdas With Bindings And Arguments

```cpp
// highlight-word:(int & refI)
#include <print>

int main() {
    int i = 41;
    // bind by value
    auto print_1 = [=] (int & refI) { std::println("i: {0}, refI: {1}", i, refI); };
    ++i;
    // bind by value again
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

# Lambdas Individual Binding  

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

# Lambdas Binding to New Name  

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


---
transition: fade-out
layoutClass: gap-16
layout: two-cols
---

```cpp
int main() {}
````

::right::

![](/images/IMG_7223.JPEG)

---
layout: quote
background: /images/DSC_0648.JPG
---

I love Deadlines. 

I love the whooshy noise they make as they pass by.

---
layout: two-cols
layoutClass: gap-16
---

# Font Demo

This slide demonstrates the **Atkinson Hyperlegible Next** font — designed for accessibility and legibility.

**Bold text** and *italic text* render clearly. Compare easily confusable characters:

- `Il1|` — uppercase I, lowercase l, digit 1, pipe
- `O0Oo` — uppercase O, digit 0, lowercase o
- `B8` — letter B, digit 8

::right::

# Code Font:<br>Intel One Mono

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    std::for_each(v.begin(), v.end(),
        [](int& n) { n *= 2; });

    for (auto x : v)
        std::cout << x << ' ';
    // Output: 2 4 6 8 10
}
```

---

# C++11: Capturing `this`

In C++11, lambdas can capture `this` — but only as a **pointer**:

```cpp {all|5-7|all}
struct Widget {
    int value = 42;

    void process() {
        auto lambda = [this]() {
            return value;  // accesses this->value
        };
        std::cout << lambda() << '\n';
    }
};
```

<v-click>

**Problem:** If `Widget` is destroyed before the lambda runs, you get **dangling pointer** access.

</v-click>

<!--
This is the genesis: C++11 lambdas could only capture this as a pointer.
-->

---

# C++17: Capturing `*this` by Copy

C++17 added `*this` capture — the lambda gets its **own copy** of the object:

````md magic-move {lines: true}
```cpp
// C++11/14: captures 'this' pointer — dangling risk
auto lambda = [this]() {
    return value;
};
```

```cpp
// C++17: captures '*this' by value — safe copy
auto lambda = [*this]() {
    return value;  // accesses the copy
};
```

```cpp
// C++17: captures '*this' by value, needs mutable to modify
auto lambda = [*this]() mutable {
    value = 99;  // modifies the lambda's copy, not the original
    return value;
};
```
````

<v-click>

This is the key change! The lambda owns an **independent copy** of the object.

</v-click>

<!--
Magic Move animates between the code blocks to show the evolution.
-->

---

# C++20: Deprecating Implicit `this` in `[=]`

C++20 deprecated the implicit capture of `this` via `[=]` (P0806R2):

```cpp {all|3|4|5|all}
struct Widget {
    int value = 42;
    void f() {
        auto a = [=]() { return value; };        // ⚠️ deprecated: captures this pointer
        auto b = [=, this]() { return value; };   // ✅ explicit this pointer (C++20)
        auto c = [=, *this]() { return value; };  // ✅ explicit copy of *this (C++17)
    }
};
```

<v-click>

**Why?** `[=]` suggests "everything by value", but `this` is captured as a **pointer** (by reference, effectively). This is confusing and error-prone.

</v-click>

<v-click>

`[=, this]` was made valid in C++20 (P0409R2) to provide a clean upgrade path.

</v-click>

<!--
The deprecation resolves the inconsistency: [=] looked like value capture but captured this by reference.
-->

---

# C++23: Deducing `this` (P0847R7)

C++23 introduces **explicit object parameters** — lambdas can now refer to themselves:

````md magic-move {lines: true}
```cpp
// Before C++23: recursive lambda needs std::function
std::function<int(int)> fib = [&fib](int n) -> int {
    return n <= 1 ? n : fib(n - 1) + fib(n - 2);
};
```

```cpp
// C++23: deducing this — no std::function needed
auto fib = [](this auto self, int n) -> int {
    return n <= 1 ? n : self(n - 1) + self(n - 2);
};
```
````

<v-click>

This doesn't change *capture* rules, but gives lambdas a **first-class `self` reference**.

</v-click>

<v-click>

Also in C++23: **static lambdas** (P1169R4) — `[]() static { ... }` for capture-less lambdas.

</v-click>

<!--
Deducing this is the biggest lambda change since C++11.
It enables recursive lambdas, CRTP-like patterns, and more.
-->

---
layout: center
class: text-center
background: /images/IMG_6545.JPEG
---

# The End

[Source Code](https://github.com/daixtrose/capture-this) · [Slides Online](#)

<div class="text-sm opacity-50 mt-4">
  C++ User Group Aachen — 2026
</div>

---

Based on [Capture *this in lambda expression: Timeline of change](https://www.nextptr.com/tutorial/ta1430524603/capture-this-in-lambda-expression-timeline-of-change)
