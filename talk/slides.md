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
src: ./lambda_basics.md
---

---

# Lambda Inside a Struct/Class

````md magic-move {lines: true}
```cpp
#include <iostream>

int main() {
    int i = 42;
    auto print = [=] () { std::cout << "i: " << i << '\n'; };
    print();
}
```

```cpp
#include <iostream>

struct C {
    int i;

    void process() {
        auto print = [] () { std::cout << "i: " << i << '\n'; };
        print();
    }
};  

int main() {
    C c{42};
    c.process();
}
```

<div v-show="$clicks === 0">
<p>Output of program:</p>
<pre><code>i: 42</code></pre>
</div>
````

---

# Lambda Inside a Struct/Class

```cpp
#include <iostream>

struct C {
    int i;
    void process() {
        auto print = [] () { std::cout << "i: " << i << '\n'; };
        print();
    }
};  
```

```
<source>: In lambda function:
<source>:7:52: error: 'this' was not captured for this lambda function
    7 |         auto print = [] () { std::cout << "i: " << i << '\n'; };
      |                                                    ^
<source>:7:52: error: invalid use of non-static data member 'C::i'
<source>:4:9: note: declared here
    4 |     int i;
      |         ^
```

---

# Lambda Inside a Struct/Class: C++11

```cpp
// highlight-word:[&]
#include <iostream>

struct C {
    int i;
    void process() {
        auto print = [&] () { std::cout << "i: " << i << '\n'; };
        print();
    }
};  

int main() { C c{42}; c.process(); }
```

Output

```
i: 42
```

---

# Lambda Inside a Struct/Class: C++11

```cpp
// highlight-word:[=]
#include <iostream>

struct C {
    int i;
    void process() {
        auto print = [=] () { std::cout << "i: " << i << '\n'; };
        print();
    }
};  

int main() { C c{42}; c.process(); }
```

Output

```
i: 42
```

---

# Lambda Inside a Struct/Class: C++11

````md magic-move {lines: true}
```cpp
#include <iostream>

struct C {
    int i;
    void process() {
        auto print = [this] () { std::cout << "i: " << this->i << '\n'; };
        //                                               ^
        //                                               | 
        print();
    }
};
int main() { C c{42}; c.process();   
```

```cpp
#include <iostream>

struct C {
    int i;
    void process() {
        auto print = [this] () { std::cout << "i: " << i << '\n'; };
        //              ^                              ^
        //              |                              |                                                
        print();
    }
};
int main() { C c{42}; c.process();   
```
````

Output

```
i: 42
```

`this` is captured ***by value***, i.e. as ***pointer***. 

---

# C++11

````md magic-move {lines: true}
```cpp
struct C {
    int i;
    void process() {
        auto print = [this] () { std::cout << "i: " << i << '\n'; };
        print();
        ++i;
        print();
    }
};  
int main() { C c{42}; c.process(); }
```

```cpp
struct C {
    int i;
    void process() {
        auto print = [=] () { std::cout << "i: " << i << '\n'; };
        print();
        ++i;
        print();
    }
};  
int main() { C c{42}; c.process(); }
```
````

<div v-show="$clicks === 1">
<p>WTF: <code>this</code> is captured by value (aka pointer), and <code>i</code> is <code>this->i</code>.</p>
<pre><code>i: 42 
i: 43</code></pre>
</div>

---

<div style="height: 100%; display: flex; align-items: center; justify-content: center">
  <img src="/images/IMG_7223.JPEG" style="height: 80%">
</div>

---
background: /images/IMG_9009.JPEG
---

# 3 Years Later ...

---

# How do I capture this by copy?

Idea: use `*this` in capture arguments.

```cpp
// highlight-word:*this
struct C { 
    int i;
    void process() {
        auto print = [*this] () { std::cout << "i: " << i << '\n'; };
        print(); 
    } 
};  
```

---

# C++11: Cannot capture `this` by copy?

```cpp
// highlight-word:*this
struct C { 
    int i;
    void process() {
        auto print = [*this] () { std::cout << "i: " << i << '\n'; };
```

Compiler output for `gcc-6.1 -std=c++11`:

```
<source>: In member function 'void C::process()':
<source>:7:23: error: expected identifier before '*' token
         auto print = [*this] () { std::cout << "i: " << i << '\n'; };
                       ^
<source>: In lambda function:
<source>:7:57: error: 'this' was not captured for this lambda function
         auto print = [*this] () { std::cout << "i: " << i << '\n'; };
                                                         ^
<source>:7:57: error: invalid use of non-static data member 'C::i'
<source>:4:9: note: declared here
     int i;
         ^
Compiler returned: 1
```

---

# C++17: Capture `*this` for a Copy!

````md magic-move {lines: true}
```cpp
#include <iostream>

struct C {
    int i;
    void process() {
        auto print = [*this] () { 
            std::cout << "i: " << i << '\n'; };
        print(); i+=10; /* <- has no effect*/ print();
    }
};  
int main() { C c{42}; c.process(); }
```

```cpp
#include <iostream>

struct C {                   // C++11 feature
    int i;                   //   |          
    void process() {         //   V
        auto print = [*this] () mutable { 
            ++i; std::cout << "i: " << i << '\n'; };
        print(); i+=10; /* <- has no effect*/ print();
    }
};  
int main() { C c{41}; c.process(); }
```
````

Output (when compiled with `gcc-7.5 -std=c++17`):

<div v-show="$clicks === 0">
<pre><code>i: 42
i: 42</code></pre>
</div>

<div v-show="$clicks === 1">
<pre><code>i: 42
i: 43</code> (inner increment has effect!)</pre>
</div>

---

# XXX


```cpp


```

---
layout: quote
background: /images/DSC_0648.JPG
---

Explicit is better than implicit.

---
background: /images/IMG_9009.JPEG
---

# 3 Years Later ...

---

# C++20: Deprecating Implicit `this` in `[=]`

C++20 deprecated the implicit capture of `this` via `[=]` (P0806R2):

```cpp {all|4,5|6,7|8,9|all}
struct C {
    int i = 42;
    void f() {
        // ⚠️ deprecated: captures this pointer
        auto a = [=]() { return i; }; 
        // ✅ explicit copy of *this (C++17)  
        auto c = [=, *this]() { return i; };  
        // ✅ explicit this pointer (C++20)       
        auto b = [=, this]() { return i; }; 
    }
};
```

<div v-show="$clicks === 1">
<p><b>Why?</b> <code>[=]</code> suggests "everything by value", but <code>this</code> is captured as a **pointer**. This is confusing and error-prone.</p>
</div>

<div v-show="$clicks === 3">
<p><code>[=, this]</code> made valid in C++20 (P0409R2) to provide a clean upgrade path.</p>
</div>



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
