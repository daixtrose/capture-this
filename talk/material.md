# Material: Capturing `this` and Deducing `this` in C++ Lambdas

Curated list of WG21 proposals, standard references, and articles relevant to the talk.

---

## WG21 Proposals — Capturing `this`

### C++11: Lambda Expressions (Original)

| Paper | Title | Status |
|-------|-------|--------|
| [N2550](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2550.pdf) | Lambda Expressions and Closures for C++ | Accepted into C++11 |
| [N2927](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2927.pdf) | New wording for C++0x Lambdas (rev. 2) | Final wording for C++11 |

### C++14: Init-Capture (Generalized Lambda Capture)

| Paper | Title | Status |
|-------|-------|--------|
| [N3610](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3610.html) | Generic Lambdas and Init-Capture | Motivation discussion |
| [N3648](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3648.html) | Wording Changes for Generalized Lambda-capture | Accepted into C++14 |

Init-capture enables the `[self = *this]` workaround for capturing `*this` by value.

### Capturing `*this` by Value (towards C++17)

| Paper | Title | Authors | Status |
|-------|-------|---------|--------|
| [N3424](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3424.pdf) | Lambda Capture of `*this` by Value | H. Carter Edwards | Early exploration |
| [P0018R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0018r0.html) | Lambda Capture of `*this` by Value as `[=,*this]` | H. Carter Edwards et al. | R0 |
| [P0018R1](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0018r1.html) | Lambda Capture of `*this` by Value as `[=,*this]` | H. Carter Edwards et al. | R1 |
| [P0018R3](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0018r3.html) | Lambda Capture of `*this` by Value as `[=,*this]` | H. Carter Edwards, D. Vandevoorde, C. Trott, H. Finkel, J. Reus, R. Maffeo, B. Sander | **Accepted into C++17** |

### Allow `[=, this]` (towards C++20)

| Paper | Title | Author | Status |
|-------|-------|--------|--------|
| [P0409R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0409r0.html) | Allow lambda capture `[=, this]` | Thomas Köppe | R0 |
| [P0409R2](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0409r2.html) | Allow lambda capture `[=, this]` | Thomas Köppe | **Accepted into C++20** |

Makes `[=, this]` legal so users can explicitly document their intent. Previously `[=, this]` was ill-formed.

### Deprecate Implicit Capture of `this` via `[=]` (towards C++20)

| Paper | Title | Author | Status |
|-------|-------|--------|--------|
| [P0806R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0806r0.html) | Deprecate implicit capture of `this` via `[=]` | Thomas Köppe | R0 (also proposed deprecating `[&]` implicit capture) |
| [P0806R2](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0806r2.html) | Deprecate implicit capture of `this` via `[=]` | Thomas Köppe | **Accepted into C++20** |

EWG in Albuquerque retained `[&]` as idiomatic and only deprecated `[=]` implicit `this` capture.

---

## WG21 Proposals — Deducing `this`

| Paper | Title | Authors | Status |
|-------|-------|---------|--------|
| [P0847R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0847r0.html) | Deducing this | G. Ažman, S. Brand, B. Deane, B. Revzin | R0 (Rapperswil 2018) |
| [P0847R1](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0847r1.html) | Deducing this | G. Ažman, S. Brand, B. Deane, B. Revzin | R1 (San Diego 2018) |
| [P0847R2](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0847r2.html) | Deducing this | G. Ažman, S. Brand, B. Deane, B. Revzin | R2 (Kona 2019) |
| [P0847R7](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html) | Deducing this | G. Ažman, S. Brand, B. Deane, B. Revzin | **Accepted into C++23** |

Introduces explicit object parameters (`this auto&& self`) enabling:
- Deduplication of const/ref-qualified overloads
- CRTP without templates
- Recursive lambdas
- By-value member functions
- SFINAE-friendly callables

### Related Proposals

| Paper | Title | Status |
|-------|-------|--------|
| [P0839R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0839r0.html) | Recursive Lambdas (Richard Smith) | Superseded by P0847 |
| [P1169R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1169r0.html) | `static operator()` (Barry Revzin, Casey Carter) | Related to by-value `this` |

---

## C++ Standard Draft (GitHub)

- **Repository:** https://github.com/cplusplus/draft
- Key sections: `[expr.prim.lambda.capture]`, `[dcl.fct]` (explicit object parameters)

---

## cppreference.com

- [Lambda expressions](https://en.cppreference.com/w/cpp/language/lambda) — comprehensive reference covering all standard versions
- [Member functions — explicit object parameter](https://en.cppreference.com/w/cpp/language/member_functions#Explicit_object_parameter) — C++23 deducing `this`

---

## Articles and Blog Posts

### Primary Reference Article

- **Capture `*this` in lambda expression: Timeline of change** — nextptr (hkumar, Jun 2020)
  https://www.nextptr.com/tutorial/ta1430524603/capture-this-in-lambda-expression-timeline-of-change
  Covers C++11 through C++20 changes with code examples and a summary table.

### By Proposal Authors / Committee Members

- **Barry Revzin** — co-author of P0847 (Deducing this)
  - Blog: https://brevzin.github.io/
  - "Deducing this Patterns" series covers recursive lambdas, CRTP replacement, and more.

- **Sy Brand** — co-author of P0847
  - "Deducing this" CppCon talk materials and blog posts.
  - Blog: https://devblogs.microsoft.com/cppblog/author/sibrand/ (Microsoft C++ blog)

- **Herb Sutter** — "GotW" and "Sutter's Mill"
  - https://herbsutter.com/
  - Discusses lambda best practices and upcoming features.

- **Thomas Köppe** — author of P0409 and P0806
  - Papers themselves serve as excellent explanatory documents with rationale and alternatives discussion.

### Conference Talks

- **"Deducing this" — Ben Deane & Barry Revzin** (CppCon)
  Covers the full motivation, design, and real-world examples of P0847.

- **"Lambda Expressions in C++" — various CppCon/Meeting C++ speakers**
  Multiple talks cover the evolution from C++11 to C++20.

### Other Useful References

- **Jason Turner (C++ Weekly)**
  - Episode on `[*this]` capture: https://www.youtube.com/c/lefticus — search for "capture this"
  - Episode on deducing this (C++23)

- **Abseil Tip of the Week #1** — https://abseil.io/tips/1
  Referenced in P0847 regarding passing small types by value.

- **Scott Meyers, *Effective C++*, Item 3**
  Referenced in P0847 for the `const_cast` delegation pattern that deducing `this` eliminates.

---

## Timeline Summary

| Standard | Feature | Key Paper |
|----------|---------|-----------|
| C++11 | Lambda expressions, `[this]` captures pointer | N2550/N2927 |
| C++14 | Init-capture (`[self = *this]` workaround) | N3648 |
| C++17 | `[*this]` capture by value | **P0018R3** |
| C++20 | `[=, this]` allowed | **P0409R2** |
| C++20 | `[=]` implicit `this` capture deprecated | **P0806R2** |
| C++23 | Deducing `this` (explicit object parameter) | **P0847R7** |
