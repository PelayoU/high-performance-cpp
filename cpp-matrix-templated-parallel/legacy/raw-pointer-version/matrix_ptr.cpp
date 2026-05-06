#include "matrix_ptr.hpp"
#include <ostream>  
#include <cassert>
#include <iomanip>



matrix_ptr::matrix_ptr(int nf, int nc):nfilas(nf), ncolumnas(nc), reservaMemoria(nf*nc){
        
    if (nfilas <= 0 || ncolumnas <= 0) {
        nfilas = ncolumnas = reservaMemoria = 0;
        buffer = nullptr;
    } else {
        buffer = new double[nfilas * ncolumnas]; //Creamos objeto en el heap, nuestro puntero buffer apunta a el
        for (int i = 0; i < nfilas * ncolumnas; ++i) buffer[i] = 0.0; //Rellenamos con 0
    }
}



    //Constructor por defecto es una matriz vacia
matrix_ptr::matrix_ptr() : nfilas(0), ncolumnas(0), buffer(nullptr) {}


    

matrix_ptr::~matrix_ptr(){

    delete[] buffer; //Si no hacemos esto al borrar la clase no se borrara el espacio del heap y tendre fugas de memoria,
                    //Se borrara el puntero pero no se borrara el espacio en el heap al que apunta

}


matrix_ptr::matrix_ptr(const matrix_ptr& origen)
    : nfilas(origen.nfilas), ncolumnas(origen.ncolumnas), buffer(nullptr), reservaMemoria(origen.reservaMemoria){
    
    if (origen.buffer && reservaMemoria>0) {
        buffer = new double[reservaMemoria];
        for (int i = 0; i < nfilas * ncolumnas; ++i) {
            buffer[i] = origen.buffer[i];
        }
    } else {
        // Por al construir la matriz lo que copio en una matriz vacia.
        nfilas = ncolumnas = 0;
        buffer = nullptr;
        reservaMemoria =0;
    }
}

// Constructor de movimiento:
matrix_ptr::matrix_ptr(matrix_ptr&& origen) noexcept
    : nfilas(origen.nfilas),ncolumnas(origen.ncolumnas),buffer(origen.buffer),reservaMemoria(origen.reservaMemoria) {
    origen.nfilas = 0;
    origen.ncolumnas = 0;
    origen.reservaMemoria = 0;
    origen.buffer = nullptr;
}


// Asignacion por movimiento: libera lo propio, “roba” y vacía el origen
matrix_ptr& matrix_ptr::operator=(matrix_ptr&& origen) noexcept {
    if (this == &origen) return *this;  //Aqui no hacemos nada, te estas moviendo a ti mismo

    // Liberar lo que ya tuviera en el heap
    delete[] buffer; //Liberamos 


    // Robamos puntero y atributos
    nfilas     = origen.nfilas;
    ncolumnas  = origen.ncolumnas;
    buffer     = origen.buffer;
    reservaMemoria = origen.reservaMemoria;

    // Vaciamos el origen
    origen.nfilas = 0;
    origen.ncolumnas = 0;
    origen.buffer = nullptr;

    return *this;
}


matrix_ptr matrix_ptr::operator+(const matrix_ptr& matrizB) const {
    // Deben tener la misma forma
    assert(nfilas == matrizB.nfilas && ncolumnas == matrizB.ncolumnas); //Tienen que tener las mismas dimensiones 
    //si no tienen las mismas dimensiones pongo una asercion para que de error al sumar

    // Si la primera esta vacia y la segunda tiene la misma dimension, la suma sera una vacia, la devolvemos vacia
    if (nfilas == 0 || ncolumnas == 0) {
        return matrix_ptr{};
    }

    matrix_ptr resultado(nfilas, ncolumnas); 

    // Sumamos elemento a elemento
    for (int i = 0; i < (nfilas * ncolumnas); ++i) {
        resultado.buffer[i] = this->buffer[i] + matrizB.buffer[i];
    }
    return resultado; // RVO/move
}


matrix_ptr matrix_ptr::operator*(const matrix_ptr& matrizB) const {
    //El numero de columnas de la primera matriz tiene que ser igual al numero de columnas de la segunda matriz
    assert(ncolumnas == matrizB.nfilas);

    // Si alguna esta vacia la devolvemos vacia
    if (nfilas == 0 || ncolumnas == 0 || matrizB.ncolumnas == 0) {
        return matrix_ptr{};
    }


    //Creamos la matriz que devolveremos 
    matrix_ptr result(nfilas, matrizB.ncolumnas);
    

    
    for (int i = 0; i < nfilas; ++i) {
        for (int j = 0; j < matrizB.ncolumnas; ++j) {
            double sum = 0.0;
            for (int k = 0; k < ncolumnas; ++k) {
                sum += (*this)(i,k) * matrizB(k,j);
            }
            result(i,j) = sum;
        }
    }

    return result;
}





matrix_ptr& matrix_ptr::operator=(const matrix_ptr& origen) {
    if (this == &origen) return *this;  // Por si es una autoasignacion

    //Caso en el que haya metido una matriz incorrecta, aunque en el constructor ya lo imposibilite, es
    //seguridad extra.
    if (!origen.buffer || origen.nfilas <= 0 || origen.ncolumnas <= 0) {
        delete[] buffer;
        buffer = nullptr;
        nfilas = ncolumnas = 0;
        reservaMemoria =0;
        return *this;
    }

    delete[] buffer;
    nfilas= origen.nfilas;
    ncolumnas= origen.ncolumnas;
    reservaMemoria = origen.reservaMemoria;
    
    if (nfilas > 0 && ncolumnas > 0) {
        
        buffer = new double[reservaMemoria];
        for (int i = 0; i < nfilas * ncolumnas; ++i) {
            buffer[i] = origen.buffer[i];
        }
    } else {
        nfilas = ncolumnas = 0;
        reservaMemoria =0;
        buffer = nullptr;
    }

    return *this;
}


// Operador () para acceder como m(i,j)

double& matrix_ptr::operator()(int i, int j) {
    assert(i >= 0 && i < nfilas);
    assert(j >= 0 && j < ncolumnas);
    return buffer[i * ncolumnas + j];
}

const double& matrix_ptr::operator()(int i, int j) const {  
    assert(i >= 0 && i < nfilas);
    assert(j >= 0 && j < ncolumnas);
    return buffer[i * ncolumnas + j];
}


void matrix_ptr::reservar(int n, int m){


            assert(n > 0 && m > 0); //Comprobamos que los tamaños sean validos
 
            if (reservaMemoria >= n * m) return; //No podemos reservar menos de lo que ya tiene
            
            double* nuevo = new double[n * m]; //Creamos un nuevo array en el heap con el nuevo tamaño

            for (int i = 0; i < nfilas * ncolumnas; ++i) nuevo[i] = buffer[i];
            

            delete[] buffer; //Liberamos el bloque antiguo del heap

            //Actualizamos atributos 
            buffer = nuevo; //actualizamos nuestro puntero, ahora apunta a nuestro nuevo espacio en el heap
            //No actualizamos el nFil ni el nCol, porque sigue teniendo nuestra matriz las mismas, lo que estoy haciendo
            //es simplmente reservar mas espacio

            //Actualizamos la reserva de memoria
            reservaMemoria = n*m;

        }


void matrix_ptr::redimensionar(int n, int m){

    assert(n > 0 && m > 0); //Estamos obligados a meter un tamaño valido (no hay matrices con filas o columnas negativas)

    if (n == nfilas && m == ncolumnas) return; //La redimension tiene que ser diferente al tamaño de la matriz para hacer algo


    // Si el nuevo tamaño cabe dentro de la reserva de memoria actual pues no necsitamos pedir mas espacio al heap,
    //rellenamos los huecos y ya esta
    if (n * m <= reservaMemoria) {

        //Mismo numero de columnas, solo crecen o reducen las filas
        if (m == ncolumnas) {
            //Si crecemos, rellenamos los huecos con 0.0
            if (n > nfilas) {
                for (int i = nfilas * ncolumnas; i < n * m; ++i) {
                    buffer[i] = 0.0;
                }
            }
            nfilas = n;
            ncolumnas = m;
            return;
        }

        // Si cambia el numero de columnas reordenamos internamente
        double* temp = new double[n * m]();
        int filas_minimo = std::min(nfilas, n);
        int cols_minimo  = std::min(ncolumnas, m);

        for (int i = 0; i < filas_minimo; ++i) {
            for (int j = 0; j < cols_minimo; ++j) {
                temp[i * m + j] = buffer[i * ncolumnas + j];
            }
        }

        //Copiamos el bloque temporal al buffer original (mismo heap)
        for (int i = 0; i < n * m; ++i) {
            buffer[i] = temp[i];
        }

        delete[] temp;
        nfilas = n;
        ncolumnas = m;
        return;
        }


    //En caso de que la dimension sea mayor a nuestra memoria reservada tenemos que reservar mas memoria
    if (n * m > reservaMemoria) {
        reservar(n, m); //reservamos mas si n*m es mayor que nuestra reserva de memoria
    }

    

    double* nuevo = new double[n * m](); //Reservamos espacio en el heap para una matriz del tamaño al que
    //queremos redimensionar, ahora si que tiene todo 0, no como antes (porque hemos puesto ())
    
    //Si pasamos de 2x2 a 3x3. tendremos pondremos un bucle que se mueva hasta 2x2, el resto seran 0.0
    int filas_minimo = std::min(nfilas, n);
    int cols_minimo  = std::min(ncolumnas, m);

    //El tamaño nuevo de nuestra matriz 
    for (int i = 0; i < filas_minimo; ++i) {
        for (int j = 0; j < cols_minimo; ++j) {
            nuevo[i * m + j] = buffer[i * ncolumnas + j]; 
        }
    }

    delete[] buffer;
    buffer = nuevo;

    //Actualizamos atributos 
    nfilas = n;
    ncolumnas = m;
    reservaMemoria = n * m; 

}



std::ostream& operator<<(std::ostream& os, const matrix_ptr& m) {

    os << std::fixed << std::setprecision(1);
    os << m.nfilas << " x " << m.ncolumnas << '\n';
    if (!m.buffer) return os;

    for (int i = 0; i < m.nfilas; ++i) {
        for (int j = 0; j < m.ncolumnas; ++j) {
            os << m.buffer[i * m.ncolumnas + j];
            if (j + 1 < m.ncolumnas) os << ' ';
        }
        if (i + 1 < m.nfilas) os << '\n';
    }
    return os;
}





