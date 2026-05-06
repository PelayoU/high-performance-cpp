# High-Performance C++

A portfolio of **modern C++ projects focused on performance and correctness**: RAII-managed data structures, smart-pointer ownership, move semantics, templates, and parallelism with `std::async` / `std::future`.

The work was developed during my **MSc in Financial Sector Technologies (UC3M)** as part of the *High-Performance C++* course.

---

## Project index

| Project | Stack | Focus |
| :--- | :--- | :--- |
| **[cpp-matrix-templated-parallel](./cpp-matrix-templated-parallel)** | C++17 · CMake · `std::unique_ptr` · `std::async` / `std::future` | Templated, RAII-managed matrix class with full Rule-of-Five, capacity controls (`reservar` / `redimensionar`), functional pipeline (`map`, `par_map`, `par_reduce`). Ships a side-by-side **legacy raw-pointer** implementation for direct RAII contrast. 17/17 CTest tests passing. |

---

## Areas of focus

- **RAII and ownership** — `std::unique_ptr<T[]>` over heap-allocated buffers, deep-copy semantics, move-only semantics, strong exception safety in copy assignment.
- **Modern C++ idioms** — Rule of Five, templates with `template<typename U>` friend declarations, `std::move` / `std::forward`, value-initialised `T{}` for generic zero-init.
- **Parallelism** — manual fan-out across worker threads with `std::async` + `std::future<T>`, lambda capture in worker bodies, associative reduction patterns.
- **CMake & CTest** — modular build (header-only library + tests + nested legacy subdirectory), `-Wall -Wextra -Wpedantic`, `Threads::Threads`.

---

## Author

**Pelayo Urzaiz**

- BSc in Applied Statistics — Universidad Complutense de Madrid (UCM)
- MSc in Financial Technologies (FinTech) — Universidad Carlos III de Madrid (UC3M)
- MSc in Quantitative Finance — Universidad Nacional de Educación a Distancia (UNED)

[LinkedIn Profile](https://www.linkedin.com/in/pelayourzaiz/)
