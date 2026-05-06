#include <iostream>
#include "matrix.hpp"

int main()
{
    matrix<double> m1{2, 2};
    m1(0, 0) = 5.0;
    m1(0, 1) = 6.0;
    m1(1, 0) = 7.0;
    m1(1, 1) = 8.0;

    // create new matrix moving the other one
    matrix<double> m2{std::move(m1)};

    // create new empty matrix and move-assign into it
    matrix<double> m3;
    m3 = std::move(m2);

    std::cout << "matriz m1 (empty): " << std::endl;
    std::cout << m1 << std::endl;

    std::cout << "matriz m2 (empty): " << std::endl;
    std::cout << m2 << std::endl;

    std::cout << "matriz m3 (not empty): " << std::endl;
    std::cout << m3 << std::endl;

    return 0;
}
