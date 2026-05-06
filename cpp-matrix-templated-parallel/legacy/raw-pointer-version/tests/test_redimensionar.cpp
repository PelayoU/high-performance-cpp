#include <iostream>
#include "matrix_ptr.hpp"

int main()
{
    matrix_ptr m1{3, 3};
    m1(0, 0) = 1.0; m1(0, 1) = 2.0; m1(0, 2) = 3.0;
    m1(1, 0) = 4.0; m1(1, 1) = 5.0; m1(1, 2) = 6.0;
    m1(2, 0) = 7.0; m1(2, 1) = 8.0; m1(2, 2) = 9.0;

    std::cout << "matriz m1 (original 3x3): " << std::endl;
    std::cout << m1 << std::endl;

    m1.redimensionar(4, 4);
    std::cout << "matriz m1 (augmented to 4x4): " << std::endl;
    std::cout << m1 << std::endl;

    m1.redimensionar(2, 2);
    std::cout << "matriz m1 (reduced to 2x2): " << std::endl;
    std::cout << m1 << std::endl;

    return 0;
}
