---
theme: ./daixtrose-basic
title: "Capture *this — Timeline of Change"
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
transition: slide-left
colorSchema: both
toc:
  maxDepth: 1
  minDepth: 1
---

# Capture `*this`

Timeline of Change in C++ Lambda Expressions

<div class="abs-br m-6 text-sm opacity-50">
  C++ User Group Aachen
</div>

<!--
Opening slide. Welcome the audience, introduce yourself, show QR codes.
-->

---
transition: fade-out
---

# What This Talk Covers

- How capturing `this` in lambdas evolved across C++ standards
- C++11 — implicit capture of `this`
- C++14 — init captures & workarounds
- C++17 — `*this` capture by copy
- C++20 — deprecation of implicit `this` in `[=]`

<br>

Based on [Capture *this in lambda expression: Timeline of change](https://www.nextptr.com/tutorial/ta1430524603/capture-this-in-lambda-expression-timeline-of-change)

<!--
Give a brief roadmap of what we'll cover.
-->

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
layout: center
class: text-center
---

# Thank You!

[Source Code](https://github.com/daixtrose/capture-this) · [Slides Online](#)

<div class="text-sm opacity-50 mt-4">
  C++ User Group Aachen — 2026
</div>
