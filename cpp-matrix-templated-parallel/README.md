# cpp-matrix-templated-parallel

Hand-rolled **templated, RAII-managed, parallelisable matrix class** in modern C++17, built up across four iterations of a high-performance C++ course. The repository ships the **final iteration** as the headline implementation (in [`include/matrix.hpp`](include/matrix.hpp)) and keeps a **legacy raw-pointer version** alongside (in [`legacy/raw-pointer-version/`](legacy/raw-pointer-version/)) so the engineering decisions that come with `std::unique_ptr<T[]>` are visible in direct comparison.

---

## What's inside

- **`matrix<T>`** — header-only, single TU, ~500 lines: row-major storage backed by `std::unique_ptr<T[]>`, full Rule-of-Five (copy / move ctor + assign), element access via `operator()`, arithmetic (`+`, `*`), capacity controls (`reservar`, `redimensionar`), and functional pipeline (`map`, `par_map`, `par_reduce`).
- **`par_map` / `par_reduce`** — manual fan-out across `(num_cores - 1)` async workers via `std::async` + `std::future`, with the main thread mopping up the leftover rows. Falls back to the sequential path on `num_cores <= 1`.
- **`legacy/raw-pointer-version/matrix_ptr`** — the same surface, but built around `new T[]` / `delete[]` and hand-rolled destructor / copy / move semantics, kept for comparison.
- **17 CTest test executables** covering construction, copy, move, arithmetic, capacity, templated instantiation, sequential map and parallel map / reduce.

---

## Tech stack

| Area | Choice |
|------|--------|
| Language | C++17 |
| Build | CMake 3.16+ |
| Threading | `std::async`, `std::future`, `std::vector<std::future<...>>` |
| Test runner | CTest (no external test framework) |

---

## Project layout

```
cpp-matrix-templated-parallel/
├── CMakeLists.txt                  # top-level CMake; pulls in legacy/ as a subdir
├── include/
│   └── matrix.hpp                  # the headline templated implementation
├── tests/                          # 10 tests against matrix<T>
│   ├── test_inicial.cpp            # construction
│   ├── test_copia.cpp              # copy ctor + copy assignment
│   ├── test_mov.cpp                # move ctor + move assignment
│   ├── test_suma.cpp               # operator+
│   ├── test_multi.cpp              # operator*
│   ├── test_reservar.cpp           # capacity-only growth
│   ├── test_redimensionar.cpp      # logical-shape resize, growth + shrink
│   ├── test_generico.cpp           # matrix<int> AND matrix<double> co-existing
│   ├── test_map.cpp                # par_map with named function + lambda
│   └── test_reduce.cpp             # par_reduce with named function + lambda
└── legacy/
    └── raw-pointer-version/        # raw `new[]/delete[]` implementation
        ├── CMakeLists.txt
        ├── README.md               # raw vs smart-pointer side-by-side table
        ├── matrix_ptr.hpp
        ├── matrix_ptr.cpp
        └── tests/                  # 7 mirror tests against matrix_ptr
```

---

## Quick start

```bash
cmake -B build
cmake --build build -j
ctest --test-dir build --output-on-failure
```

Expected output: **17/17 tests pass** (10 templated + 7 legacy raw-pointer).

A Release build is selected by default; pass `-DCMAKE_BUILD_TYPE=Debug` for assertion-friendly debugging — every accessor uses `assert()` for index validation.

---

## Evolution across four phases

The class was developed in four phases; each phase folded into the next, so the final `matrix.hpp` is the strict superset of everything below.

### Phase 1 — RAII and the Rule of Five

Goal: hold a heap-allocated 2D buffer of `double` and survive the canonical lifecycle bugs.

- Two implementations side by side: **raw pointer** (`matrix_ptr` with `new double[]` / `delete[]` and a hand-written destructor) and **smart pointer** (`matrix_smart` with `std::unique_ptr<double[]>` and an implicit destructor).
- Copy ctor + copy assignment do a deep copy (the obvious mistake: forgetting to allocate a fresh buffer leaves both matrices sharing storage and sets up a double-free).
- Move ctor + move assignment steal the buffer pointer and null out the source. With `unique_ptr`, the move is a single `std::move(buffer)` — the ownership transfer is the type's job, not the developer's.
- `operator+`, `operator*` for arithmetic; `operator()` for element access (with both `T&` and `const T&` overloads).

### Phase 2 — Capacity vs size

Goal: separate **logical shape** from **allocated capacity**, the same way `std::vector` does.

- `reservar(n, m)` grows the underlying allocation to at least `n*m` cells, leaving the logical `nfilas`/`ncolumnas` untouched (mirrors `std::vector::reserve`).
- `redimensionar(n, m)` resizes the logical shape; reuses existing capacity when possible (no allocation if `n*m` fits in `reservaMemoria`), only allocates when growing past capacity. New cells are zero-initialised; lost cells are dropped.

### Phase 3 — Templates

Goal: stop hardcoding `double`.

- The whole class becomes `template<typename T>`. The header collapses from `matrix.hpp` + `matrix.cpp` to a single header (`#include "matrix.hpp"` is everything you need).
- `operator<<` becomes a `friend` declared with its own template parameter `U` so it can be instantiated independently from any given `matrix<T>` specialisation.
- `T{}` replaces `0.0` for default-initialising new cells — works for `int`, `double`, `std::complex<float>`, anything regular.
- `map(func)` is the first functional combinator: returns a new matrix with `func` applied element-wise. Const-qualified, doesn't mutate this matrix.

### Phase 4 — Parallelism

Goal: parallelise `map` and add `reduce`.

- **`par_map(func, num_cores)`** — fans out the rows across `num_cores - 1` async workers; the main thread takes the leftover rows that don't divide evenly. Once every future returns, the result matrix is complete.
- **`par_reduce(func, init, num_cores)`** — each worker reduces its slice locally, the main thread reduces the leftover rows, and finally every worker's partial result is folded into a single accumulator with the same `func`. Requires associativity of `func` for deterministic results.
- Both functions fall back to the sequential path when `num_cores <= 1`, which makes them trivially safe in single-core test runners.

---

## Why the legacy raw-pointer version is kept

The contrast between [`legacy/raw-pointer-version/matrix_ptr.cpp`](legacy/raw-pointer-version/matrix_ptr.cpp) (Phase 1+2 surface) and [`include/matrix.hpp`](include/matrix.hpp) (Phase 4 surface) is the most concrete answer the repo can give to *"why RAII?"*.

| Concern | Raw-pointer version | Templated smart-pointer version |
| :--- | :--- | :--- |
| Allocation | `buffer = new double[n*m]` | `buffer = std::make_unique<T[]>(n*m)` |
| Destruction | Explicit `~matrix_ptr() { delete[] buffer; }` | Implicit — `unique_ptr` calls `delete[]` automatically |
| Move ctor | Manually copy pointer + null out source's pointer | `buffer(std::move(origen.buffer))` — `unique_ptr` does both |
| Copy assignment | Must `delete[] buffer` before reallocating, otherwise leak on second assignment | Strong exception safety idiom: stage in a local `unique_ptr`, then move-swap |
| Exception safety | A throw between `new` and the next `delete[]` leaks | `unique_ptr` releases automatically when its scope unwinds |
| Genericity | Hardcoded to `double` | `template<typename T>` — `matrix<int>`, `matrix<std::complex<double>>`, ... |

The raw-pointer code also passes the same set of tests (`legacy_test_*` in CTest), so the comparison is *behavioural equivalence vs implementation cost* — which is the cleanest way to motivate the move from "correct C++" to "correct, robust C++".

---

## Reference

Built in the context of the *High-Performance C++* track, MSc in Financial Sector Technologies (UC3M, 2024/2025).
