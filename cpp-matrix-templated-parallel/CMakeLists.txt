cmake_minimum_required(VERSION 3.16)
project(cpp_matrix_templated_parallel
        VERSION 1.0.0
        DESCRIPTION "Templated, RAII-managed matrix with parallel map/reduce"
        LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Compiler / language settings
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Sensible warnings on every compiler we care about.
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Threading: par_map / par_reduce use std::async, which needs pthread on
# most Unixes. CMake's Threads module abstracts the linker flag.
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# -----------------------------------------------------------------------------
# Library: header-only matrix
# -----------------------------------------------------------------------------
add_library(matrix INTERFACE)
target_include_directories(matrix INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(matrix INTERFACE Threads::Threads)

# -----------------------------------------------------------------------------
# Tests (CTest)
# -----------------------------------------------------------------------------
enable_testing()

set(MATRIX_TESTS
    test_inicial
    test_copia
    test_mov
    test_suma
    test_multi
    test_reservar
    test_redimensionar
    test_generico
    test_map
    test_reduce
)

foreach(t IN LISTS MATRIX_TESTS)
    add_executable(${t} tests/${t}.cpp)
    target_link_libraries(${t} PRIVATE matrix)
    add_test(NAME ${t} COMMAND ${t})
endforeach()

# -----------------------------------------------------------------------------
# Legacy raw-pointer subdirectory
# -----------------------------------------------------------------------------
add_subdirectory(legacy/raw-pointer-version)
