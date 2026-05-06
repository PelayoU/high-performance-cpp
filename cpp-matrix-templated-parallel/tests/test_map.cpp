#include <iostream>
#include "matrix.hpp"

int duplicate (int a) {
    return 2*a;
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

    // map to duplicate
    auto m2 = m1.par_map(duplicate,2);
    
    // print the matrix original
    std::cout << "matriz m1 (original): " << std::endl;
    std::cout << m1 << std::endl;

    // print the duplicated matrix
    std::cout << "matriz m2 (duplicate): " << std::endl;
    std::cout << m2 << std::endl;

    // declare new matrix
    matrix<double> m3{2,2};
    
    // fill up the matrix
    m3(0,0) = 1.1;
    m3(0,1) = 2.2;
    m3(1,0) = 3.3;
    m3(1,1) = 4.4;

    // map to duplicate with lambda
    auto m4 = m3.par_map([] (double a) {return 2.0*a;}, 2);

    // print the new matrix
    std::cout << "matriz m3 (original): " << std::endl;
    std::cout << m3 << std::endl;

    // print the duplicated matrix
    std::cout << "matriz m4 (duplicate): " << std::endl;
    std::cout << m4 << std::endl;

    return 0;
}
