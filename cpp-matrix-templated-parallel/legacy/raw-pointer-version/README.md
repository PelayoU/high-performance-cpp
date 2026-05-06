# Legacy: raw-pointer `matrix_ptr`

This directory keeps the **early-phase implementation** of the matrix as a hand-managed `double*` buffer with explicit `new[]` / `delete[]`, alongside the same suite of tests that the templated version supports.

It exists for one reason: **direct comparison** with the templated, smart-pointer version in [`include/matrix.hpp`](../../include/matrix.hpp). The two implementations expose the same public API (constructors, `operator+`, `operator*`, `reservar`, `redimensionar`, …), so a reader can put the files side by side and see exactly what `std::unique_ptr<T[]>` removes from the developer's plate:

| Concern | Raw-pointer version (`matrix_ptr.cpp`) | Smart-pointer version (`matrix.hpp`) |
| :--- | :--- | :--- |
| Allocation | `buffer = new double[...]` | `buffer = std::make_unique<T[]>(...)` |
| Destruction | Custom destructor calling `delete[] buffer` | Implicit — owned by `unique_ptr` |
| Move ctor | Manually copies pointer + nulls origin | `unique_ptr`'s own move constructor handles both |
| Copy assignment | Must `delete[] buffer` before reallocating | The temporary `unique_ptr` swap is automatic |
| Exception safety | Manual: stage in a temporary, then swap | Same idiom, but a leak is impossible if anything throws mid-flight |
| Genericity | Hardcoded to `double` | `template<typename T>` — `matrix<int>`, `matrix<double>`, … |

Both versions ship with the **same tests** (`test_inicial`, `test_copia`, `test_mov`, `test_suma`, `test_multi`, `test_reservar`, `test_redimensionar`); the legacy build prefixes its targets with `legacy_` so they don't clash with the templated ones.

## Build

The legacy directory is wired into the parent CMake project — building from the project root picks it up automatically:

```bash
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

The legacy tests appear with the `legacy_` prefix in the CTest output.
