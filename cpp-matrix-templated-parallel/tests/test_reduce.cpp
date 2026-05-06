#include <iostream>
#include "matrix.hpp"

int multiply (int a, int b) {
    return a*b;
}

int main()
{
    // declare new matrix
    matrix<int> m1{2,2};
    
    // fill up the matrix
    m1(0,0) = 1;
    m1(0,1) = 2;
    m1(1,0) = 3;
    m1(1,1) = 4;

    // reduce by multiplication
    auto ret1 = m1.par_reduce(multiply,1,2);
    
    // print the matrix original
    std::cout << "matriz m1 (original): " << std::endl;
    std::cout << m1 << std::endl;

    // print the reduction of matrix
    std::cout << "reduction of matriz m1: " << std::endl;
    std::cout << ret1 << std::endl << std::endl;

    // declare new matrix
    matrix<double> m3{2,2};
    
    // fill up the matrix
    m3(0,0) = 1.1;
    m3(0,1) = 2.2;
    m3(1,0) = 3.3;
    m3(1,1) = 4.4;

    // reduce by adding with lambda
    auto ret2 = m3.par_reduce([] (double a, double b) {return a+b;}, 0, 2);

    // print the new matrix
    std::cout << "matriz m3 (original): " << std::endl;
    std::cout << m3 << std::endl;

    // print the reduction of matrix
    std::cout << "reduction of matriz m3: " << std::endl;
    std::cout << ret2 << std::endl << std::endl;

    return 0;
}
