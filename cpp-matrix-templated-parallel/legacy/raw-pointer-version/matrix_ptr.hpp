#pragma once
#include <iosfwd>

class matrix_ptr {
private:
    int nfilas;
    int ncolumnas;
    double* buffer;
    int reservaMemoria; 

public:
    matrix_ptr();                     // constructor por defecto
    matrix_ptr(int nf, int nc);       // constructor con tamaño
    ~matrix_ptr();                    // destructor


    //Constructor copia y operador de asignacion por copia
    matrix_ptr(const matrix_ptr& origen);
    matrix_ptr& operator=(const matrix_ptr& origen);
    
    //Movimiento
    matrix_ptr(matrix_ptr&& origen) noexcept;             
    matrix_ptr& operator=(matrix_ptr&& origen) noexcept; 

    //Suma (tarea 4)
    matrix_ptr operator+(const matrix_ptr& matrizB) const;


    //Multiplicacion de matrices
    matrix_ptr operator*(const matrix_ptr& matrizB) const;

    
    
    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;
    
    friend std::ostream& operator<<(std::ostream& os, const matrix_ptr& m);

    void reservar(int n, int m);
    void redimensionar(int n, int m);

};


//using matrix = matrix_ptr; 
