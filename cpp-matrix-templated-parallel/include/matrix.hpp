// =============================================================================
// matrix<T> — templated, RAII-managed, optionally parallel
// =============================================================================
// Final iteration of the lab series:
//   Lab 1: hand-rolled raw-pointer matrix with copy/move semantics.
//   Lab 2: + reservar() and redimensionar() (capacity vs size, std::vector-style).
//   Lab 3: + templated on T, + map(func).
//   Lab 4: + par_map(func, num_cores) and par_reduce(func, init, num_cores)
//          on top of std::async / std::future.
//
// This header owns the full surface area: storage, copy, move, accessors,
// arithmetic, capacity controls, functional map, parallel map / reduce.
// =============================================================================

#pragma once

#include <iosfwd>
#include <memory>
#include <ostream>
#include <cassert>
#include <algorithm>
#include <iomanip>

// Concurrency: par_map / par_reduce are built on async + future.
#include <future>
#include <vector>


// -----------------------------------------------------------------------------
// Class declaration
// -----------------------------------------------------------------------------

template<typename T>
class matrix {
private:
    int                  nfilas;        // number of rows
    int                  ncolumnas;     // number of columns
    std::unique_ptr<T[]> buffer;        // owns the row-major storage
    int                  reservaMemoria;// allocated capacity (>= nfilas*ncolumnas)

public:
    // ---- Functional pipeline -------------------------------------------------
    // Apply `func` element-wise; returns a new matrix of the same shape.
    template<typename Func>
    matrix<T> map(Func func) const;

    // Same shape as map, but spreads work across (num_cores - 1) async workers
    // plus the main thread, which handles the leftover rows.
    template<typename Func>
    matrix<T> par_map(Func func, int num_cores) const;

    // Parallel reduction: applies func(elem, acc) over every element. Returns
    // a single T. The accumulation is associative across the threads.
    template<typename F>
    T par_reduce(F func, T valor_inicial, int num_cores);

    // ---- Lifecycle -----------------------------------------------------------
    matrix();                                  // empty
    matrix(int nf, int nc);                    // zero-initialised nf x nc

    matrix(const matrix<T>& origen);           // deep copy
    matrix<T>& operator=(const matrix<T>& origen);

    matrix(matrix<T>&& origen) noexcept;       // moves the unique_ptr
    matrix<T>& operator=(matrix<T>&& origen) noexcept;

    // ---- Element access ------------------------------------------------------
    T&       operator()(int i, int j);
    const T& operator()(int i, int j) const;

    // ---- Arithmetic ----------------------------------------------------------
    matrix<T> operator+(const matrix<T>& matrizB) const;
    matrix<T> operator*(const matrix<T>& matrizB) const;

    // operator<< is a free function that needs access to private members; it
    // is declared as a friend with its own template parameter U so it can be
    // instantiated separately from this matrix specialisation.
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const matrix<U>& m);

    // ---- Capacity controls ---------------------------------------------------
    void reservar(int n, int m);          // reserve n*m capacity (no shape change)
    void redimensionar(int n, int m);     // resize the logical shape
};


// -----------------------------------------------------------------------------
// Lifecycle
// -----------------------------------------------------------------------------

template<typename T>
matrix<T>::matrix(int nf, int nc)
    : nfilas(nf), ncolumnas(nc), reservaMemoria(nf * nc) {

    if (nfilas <= 0 || ncolumnas <= 0) {
        nfilas = ncolumnas = 0;
        buffer.reset();
        reservaMemoria = 0;
    } else {
        buffer = std::make_unique<T[]>(reservaMemoria);
        // std::fill is in <algorithm>: writes T{} into every cell of the new
        // array. T{} is the value-initialised default of T, which is 0 for
        // arithmetic types.
        std::fill(buffer.get(), buffer.get() + reservaMemoria, T{});
    }
}

template<typename T>
matrix<T>::matrix() : nfilas(0), ncolumnas(0), buffer(nullptr), reservaMemoria(0) {}

// Destructor is implicit: the unique_ptr cleans up the heap allocation.

// Copy constructor: deep copy. We allocate a fresh buffer of the same capacity
// and copy element-by-element. Otherwise both matrices would share storage
// and a destructor would double-free.
template<typename T>
matrix<T>::matrix(const matrix<T>& origen)
    : nfilas(origen.nfilas), ncolumnas(origen.ncolumnas),
      buffer(nullptr), reservaMemoria(origen.reservaMemoria) {

    if (origen.buffer && nfilas > 0 && ncolumnas > 0) {
        buffer = std::make_unique<T[]>(reservaMemoria);
        for (int i = 0; i < nfilas * ncolumnas; ++i) {
            buffer[i] = origen.buffer[i];
        }
    } else {
        nfilas = ncolumnas = 0;
        reservaMemoria = 0;
        buffer.reset();
    }
}

// Move constructor: steals the unique_ptr from `origen`. After the move,
// `origen` is left in a valid empty state — its unique_ptr is automatically
// nulled by std::unique_ptr's own move constructor.
template<typename T>
matrix<T>::matrix(matrix<T>&& origen) noexcept
    : nfilas(origen.nfilas), ncolumnas(origen.ncolumnas),
      buffer(std::move(origen.buffer)),
      reservaMemoria(origen.reservaMemoria) {
    origen.nfilas = 0;
    origen.ncolumnas = 0;
    origen.reservaMemoria = 0;
}

// Move assignment: same idea, plus the self-assignment guard.
template<typename T>
matrix<T>& matrix<T>::operator=(matrix<T>&& origen) noexcept {
    if (this == &origen) return *this;

    nfilas         = origen.nfilas;
    ncolumnas      = origen.ncolumnas;
    buffer         = std::move(origen.buffer);
    reservaMemoria = origen.reservaMemoria;

    origen.nfilas = 0;
    origen.ncolumnas = 0;
    origen.reservaMemoria = 0;

    return *this;
}

// Copy assignment: strong exception safety. We allocate the new buffer in a
// local unique_ptr first; if the allocation throws, this matrix is untouched.
// Only after the copy succeeds do we swap the local buffer into our own.
template<typename T>
matrix<T>& matrix<T>::operator=(const matrix<T>& origen) {
    if (this == &origen) return *this;

    if (!origen.buffer || origen.nfilas <= 0 || origen.ncolumnas <= 0) {
        buffer.reset();
        nfilas = ncolumnas = 0;
        reservaMemoria = 0;
        return *this;
    }

    nfilas         = origen.nfilas;
    ncolumnas      = origen.ncolumnas;
    reservaMemoria = origen.reservaMemoria;

    if (nfilas > 0 && ncolumnas > 0) {
        std::unique_ptr<T[]> nuevo = std::make_unique<T[]>(reservaMemoria);
        for (int i = 0; i < nfilas * ncolumnas; ++i) {
            nuevo[i] = origen.buffer[i];
        }
        buffer = std::move(nuevo);
    } else {
        nfilas = ncolumnas = 0;
        buffer.reset();
        reservaMemoria = 0;
    }

    return *this;
}


// -----------------------------------------------------------------------------
// Element access
// -----------------------------------------------------------------------------

// Non-const overload returns a mutable reference: m(i, j) = ... works.
template<typename T>
T& matrix<T>::operator()(int i, int j) {
    assert(i >= 0 && i < nfilas);
    assert(j >= 0 && j < ncolumnas);
    return buffer[i * ncolumnas + j];
}

// Const overload — required when iterating over a const matrix.
template<typename T>
const T& matrix<T>::operator()(int i, int j) const {
    assert(i >= 0 && i < nfilas);
    assert(j >= 0 && j < ncolumnas);
    return buffer[i * ncolumnas + j];
}


// -----------------------------------------------------------------------------
// Arithmetic
// -----------------------------------------------------------------------------

template<typename T>
matrix<T> matrix<T>::operator+(const matrix<T>& matrizB) const {
    assert(nfilas == matrizB.nfilas && ncolumnas == matrizB.ncolumnas);

    if (nfilas == 0 || ncolumnas == 0) {
        return matrix<T>{};
    }

    matrix<T> resultado(nfilas, ncolumnas);
    for (int i = 0; i < (nfilas * ncolumnas); ++i) {
        resultado.buffer[i] = buffer[i] + matrizB.buffer[i];
    }
    return resultado;
}

template<typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& matrizB) const {
    // Standard size compatibility for matrix multiplication.
    assert(ncolumnas == matrizB.nfilas);

    if (nfilas == 0 || ncolumnas == 0 || matrizB.ncolumnas == 0) {
        return matrix<T>{};
    }

    matrix<T> result(nfilas, matrizB.ncolumnas);

    for (int i = 0; i < nfilas; ++i) {
        for (int j = 0; j < matrizB.ncolumnas; ++j) {
            T sum = T{};
            for (int k = 0; k < ncolumnas; ++k) {
                sum += (*this)(i, k) * matrizB(k, j);
            }
            result(i, j) = sum;
        }
    }

    return result;
}


// -----------------------------------------------------------------------------
// Streaming
// -----------------------------------------------------------------------------

template<typename T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& m) {
    if (!m.buffer || m.nfilas == 0 || m.ncolumnas == 0) return os;

    for (int i = 0; i < m.nfilas; ++i) {
        for (int j = 0; j < m.ncolumnas; ++j) {
            os << m.buffer[i * m.ncolumnas + j];
            if (j + 1 < m.ncolumnas) os << ',';
        }
        if (i + 1 < m.nfilas) os << '\n';
    }
    os << '\n';
    return os;
}


// -----------------------------------------------------------------------------
// Capacity controls
// -----------------------------------------------------------------------------

// Grow the underlying storage to at least n*m cells. Logical shape is
// unchanged (the caller is just asking for headroom). Mirrors std::vector::reserve.
template<typename T>
void matrix<T>::reservar(int n, int m) {
    assert(n > 0 && m > 0);

    if (reservaMemoria >= n * m) {
        return;
    }

    reservaMemoria = n * m;
    std::unique_ptr<T[]> nuevo = std::make_unique<T[]>(reservaMemoria);

    for (int i = 0; i < nfilas * ncolumnas; ++i) {
        nuevo[i] = buffer[i];
    }

    buffer = std::move(nuevo);
}

// Resize the logical shape to (n, m), preserving overlap of the old shape
// where possible. Three regimes:
//   1) New shape fits in current capacity AND only rows change: zero-fill the
//      tail and update sizes (no allocation).
//   2) New shape fits in current capacity but columns change: rebuild the
//      row layout in a temporary, then copy back into the existing buffer.
//   3) New shape exceeds current capacity: allocate a fresh buffer.
template<typename T>
void matrix<T>::redimensionar(int n, int m) {
    assert(n > 0 && m > 0);

    if (n == nfilas && m == ncolumnas) return;

    if (n * m <= reservaMemoria) {
        if (m == ncolumnas) {
            if (n > nfilas) {
                for (int i = nfilas * ncolumnas; i < n * m; ++i) {
                    buffer[i] = T{};
                }
            }
            nfilas = n;
            ncolumnas = m;
            return;
        }

        std::unique_ptr<T[]> temp = std::make_unique<T[]>(n * m);
        int filas_minimo = std::min(nfilas, n);
        int cols_minimo  = std::min(ncolumnas, m);

        for (int i = 0; i < filas_minimo; ++i) {
            for (int j = 0; j < cols_minimo; ++j) {
                temp[i * m + j] = buffer[i * ncolumnas + j];
            }
        }

        for (int i = 0; i < n * m; ++i) {
            buffer[i] = temp[i];
        }

        nfilas = n;
        ncolumnas = m;
        return;
    }

    std::unique_ptr<T[]> nuevo = std::make_unique<T[]>(n * m);
    int filas_minimo = std::min(nfilas, n);
    int cols_minimo  = std::min(ncolumnas, m);

    for (int i = 0; i < filas_minimo; ++i) {
        for (int j = 0; j < cols_minimo; ++j) {
            nuevo[i * m + j] = buffer[i * ncolumnas + j];
        }
    }
    buffer = std::move(nuevo);

    nfilas = n;
    ncolumnas = m;
    reservaMemoria = n * m;
}


// -----------------------------------------------------------------------------
// Functional: map (sequential)
// -----------------------------------------------------------------------------

// Apply func to every element and collect the results into a new matrix of
// the same shape. Const-qualified — does not mutate this matrix.
template<typename T>
template<typename Func>
matrix<T> matrix<T>::map(Func func) const {
    matrix<T> resultado(nfilas, ncolumnas);
    for (int i = 0; i < nfilas * ncolumnas; ++i) {
        resultado.buffer[i] = func(buffer[i]);
    }
    return resultado;
}


// -----------------------------------------------------------------------------
// Functional: par_map (parallel)
// -----------------------------------------------------------------------------

// Spreads the rows across (num_cores - 1) async workers; the main thread
// finishes any leftover rows that don't divide evenly. Returns the new
// matrix once every future has been joined.
//
// Falls back to the sequential map() when num_cores <= 1.
template<typename T>
template<typename Func>
matrix<T> matrix<T>::par_map(Func func, int num_cores) const {
    if (num_cores <= 1) return map(func);

    matrix<T> result(nfilas, ncolumnas);

    int n_workers        = num_cores - 1;
    int filas_por_thread = nfilas / n_workers;

    std::vector<std::future<void>> handles;
    handles.reserve(n_workers);

    for (int core = 0; core < n_workers; core++) {
        handles.push_back(std::async(std::launch::async,
            [this, &result, func, core, filas_por_thread]() {
                for (int i = 0; i < filas_por_thread; i++) {
                    for (int j = 0; j < ncolumnas; j++) {
                        int fila = (core * filas_por_thread) + i;
                        result(fila, j) = func((*this)(fila, j));
                    }
                }
            }
        ));
    }

    // Main thread mops up the remaining rows that don't divide evenly.
    for (int i = n_workers * filas_por_thread; i < nfilas; i++) {
        for (int j = 0; j < ncolumnas; j++) {
            result(i, j) = func((*this)(i, j));
        }
    }

    for (auto& f : handles) {
        f.get();
    }

    return result;
}


// -----------------------------------------------------------------------------
// Functional: par_reduce (parallel)
// -----------------------------------------------------------------------------

// Each worker reduces its slice locally; the main thread reduces the
// leftover rows and then folds every worker's partial result into a single
// accumulator with the same `func`. The accumulation must be associative
// for the result to be deterministic across runs.
//
// Falls back to a single-threaded reduction when num_cores <= 1.
template<typename T>
template<typename F>
T matrix<T>::par_reduce(F func, T valor_inicial, int num_cores) {
    if (nfilas == 0 || num_cores <= 1) {
        T acumulado = valor_inicial;
        for (int i = 0; i < nfilas; i++) {
            for (int j = 0; j < ncolumnas; j++) {
                acumulado = func((*this)(i, j), acumulado);
            }
        }
        return acumulado;
    }

    int n_workers        = num_cores - 1;
    int filas_por_thread = nfilas / n_workers;

    std::vector<std::future<T>> handles;
    handles.reserve(n_workers);

    for (int core = 0; core < n_workers; core++) {
        handles.push_back(std::async(std::launch::async,
            [this, func, core, filas_por_thread]() -> T {
                int start_row = core * filas_por_thread;

                // Seed the local accumulator with this slice's first element
                // so we don't need an extra "neutral element" assumption.
                T local_acc = (*this)(start_row, 0);

                for (int i = 0; i < filas_por_thread; i++) {
                    for (int j = 0; j < ncolumnas; j++) {
                        int fila = (core * filas_por_thread) + i;
                        if (fila == start_row && j == 0) continue;
                        local_acc = func((*this)(fila, j), local_acc);
                    }
                }
                return local_acc;
            }
        ));
    }

    T acumulado = valor_inicial;

    // Main thread handles the leftover rows.
    int start_rem = n_workers * filas_por_thread;
    for (int i = start_rem; i < nfilas; i++) {
        for (int j = 0; j < ncolumnas; j++) {
            acumulado = func((*this)(i, j), acumulado);
        }
    }

    // Fold every worker's partial result into the global accumulator.
    for (auto& f : handles) {
        T resultado_parcial_hilo = f.get();
        acumulado = func(resultado_parcial_hilo, acumulado);
    }

    return acumulado;
}
