#include <iostream>
#include "matrix_ptr.hpp"

int main()
{
    matrix_ptr m1{2, 2};
    m1(0, 0) = 1.0;
    m1(0, 1) = 2.0;
    m1(1, 0) = 3.0;
    m1(1, 1) = 4.0;

    std::cout << "matriz m1 (original 2x2): " << std::endl;
    std::cout << m1 << std::endl;

    m1.reservar(3, 3);

    std::cout << "matriz m1 (after reservar(3,3)): " << std::endl;
    std::cout << m1 << std::endl;

    return 0;
}
