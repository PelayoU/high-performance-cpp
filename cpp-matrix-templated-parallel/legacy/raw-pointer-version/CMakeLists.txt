# =============================================================================
# Legacy raw-pointer matrix — Lab 1 + Lab 2 surface, kept for RAII contrast
# =============================================================================
# Same logical surface as the templated `matrix<T>` in `include/matrix.hpp`,
# but built around hand-managed `new T[]` / `delete[]` instead of
# std::unique_ptr<T[]>. The two implementations are kept side by side so the
# evolution from manual ownership to RAII is visible at a glance.
# =============================================================================

add_library(matrix_ptr STATIC
    matrix_ptr.cpp
)

target_include_directories(matrix_ptr PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(MATRIX_PTR_TESTS
    test_inicial
    test_copia
    test_mov
    test_suma
    test_multi
    test_reservar
    test_redimensionar
)

foreach(t IN LISTS MATRIX_PTR_TESTS)
    add_executable(legacy_${t} tests/${t}.cpp)
    target_link_libraries(legacy_${t} PRIVATE matrix_ptr)
    add_test(NAME legacy_${t} COMMAND legacy_${t})
endforeach()
