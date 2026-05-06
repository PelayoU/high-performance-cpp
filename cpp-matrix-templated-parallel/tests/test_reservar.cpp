#include <iostream>
#include "matrix.hpp"

int main()
{
    matrix<double> m1{2, 2};
    m1(0, 0) = 1.0;
    m1(0, 1) = 2.0;
    m1(1, 0) = 3.0;
    m1(1, 1) = 4.0;

    std::cout << "matriz m1 (original 2x2): " << std::endl;
    std::cout << m1 << std::endl;

    // grow the underlying storage to 3x3 worth of capacity (the logical
    // shape stays 2x2 — reservar() is std::vector::reserve, not resize).
    m1.reservar(3, 3);

    std::cout << "matriz m1 (after reservar(3,3) — capacity grew, shape didn't): " << std::endl;
    std::cout << m1 << std::endl;

    return 0;
}
