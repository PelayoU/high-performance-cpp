#include <iostream>
#include "matrix.hpp"

int main()
{
    // declare new matrix
    matrix<double> m1{2, 2};

    // fill up the matrix
    m1(0, 0) = 5.0;
    m1(0, 1) = 6.0;
    m1(1, 0) = 7.0;
    m1(1, 1) = 8.0;

    // declare new empty matrix
    matrix<double> m2;

    // print the matrixes
    std::cout << "matriz m1: " << std::endl;
    std::cout << m1 << std::endl;

    std::cout << "matriz m2: " << std::endl;
    std::cout << m2 << std::endl;

    return 0;
}
