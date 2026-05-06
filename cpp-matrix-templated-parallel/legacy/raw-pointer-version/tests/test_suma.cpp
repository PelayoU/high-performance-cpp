#include <iostream>
#include "matrix_ptr.hpp"

int main()
{
    matrix_ptr m1{2, 2};
    m1(0, 0) = 5.0;
    m1(0, 1) = 6.0;
    m1(1, 0) = 7.0;
    m1(1, 1) = 8.0;

    matrix_ptr m2{2, 2};
    m2(0, 0) = 4.0;
    m2(0, 1) = 3.0;
    m2(1, 0) = 2.0;
    m2(1, 1) = 1.0;

    matrix_ptr m3;
    m3 = m1 + m2;

    std::cout << "matriz m1: " << std::endl;
    std::cout << m1 << std::endl;

    std::cout << "matriz m2: " << std::endl;
    std::cout << m2 << std::endl;

    std::cout << "matriz m3 (m1 + m2): " << std::endl;
    std::cout << m3 << std::endl;

    return 0;
}
