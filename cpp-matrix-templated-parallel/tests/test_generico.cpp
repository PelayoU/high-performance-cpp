#include <iostream>
#include "matrix.hpp"

int main()
{
    // declare new matrix
    matrix<int> m1{2,2};
    
    // fill up the matrix
    m1(0,0) = 1;
    m1(0,1) = 2;
    m1(1,0) = 3;
    m1(1,1) = 4;

    // declare new matrix by copy
    matrix<int> m2{m1};
    
    // add two matrixes
    auto m3 = m1 + m2;

    // print the original matrix
    std::cout << "matriz m1 (original): " << std::endl;
    std::cout << m1 << std::endl;

    // print the copy matrix
    std::cout << "matriz m2 (copy): " << std::endl;
    std::cout << m2 << std::endl;

    // print the add matrix
    std::cout << "matriz m3 (add): " << std::endl;
    std::cout << m3 << std::endl;

    // declare new matrix
    matrix<double> m4{2,2};
    
    // fill up the matrix
    m4(0,0) = 1.1;
    m4(0,1) = 2.2;
    m4(1,0) = 3.3;
    m4(1,1) = 4.4;

    // declare new matrix by movement
    matrix<double> m5{std::move(m4)};
    
    // multiply  matrix by itself
    auto m6 = m5 * m5;

    // print the original matrix
    std::cout << "matriz m4 (deleted): " << std::endl;
    std::cout << m4 << std::endl;

    // print the copy matrix
    std::cout << "matriz m5 (moved): " << std::endl;
    std::cout << m5 << std::endl;

    // print the multiply matrix
    std::cout << "matriz m6 (multiply): " << std::endl;
    std::cout << m6 << std::endl;


    return 0;
}
