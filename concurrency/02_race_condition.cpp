


/*
2. Condiciones de Carrera (02_race_condition.cpp)
Objetivo: Visualizar el problema de acceder a datos compartidos sin protección. 
Enunciado: Crea una variable global int counter = 0.
 Lanza dos hilos que intenten incrementar esa variable 10,000 veces cada uno sin usar mutex. Imprime el resultado final 
(probablemente será incorrecto, distinto de 20,000). [cite_start]Referencia: Slide 15[cite: 188].
*/

#include <iostream>
#include <thread>


int contador = 0; //Variable global, fuera del main

void incremento(){
    for (int i=0; i<10000; i++){
        contador = contador +1;
    }

}

int main(){

    std::thread t1 {incremento};
    std::thread t2 {incremento};

    t1.join();
    t2.join();

    std::cout << contador; //10161, tendira que salir 20 mil. Imagina la cantidad de veces que se ha pisado

    return 0;

}

//visualizacion de como se pisan:

/*
Hilo 1 lee el contador (está en 100).

Hilo 2 lee el contador (¡también lee 100 porque el Hilo 1 aún no ha escrito!).

Hilo 1 suma 1 y escribe 101.

Hilo 2 suma 1 y escribe 101.

*/






















/*
#include <iostream>
#include <thread>

// Recurso compartido
int contador = 0; 

// Función que incrementa el contador compartido
void incrementar(int iteraciones) {
    for (int i = 0; i < iteraciones; ++i) {
        ++contador; 
    }
}

int main() {
    const int ITERACIONES = 100000;
    std::cout << "Iniciando carrera de datos (Race Condition)...\n";
    std::cout << "Esperamos llegar a: " << (ITERACIONES * 2) << "\n";

    std::thread t1(incrementar, ITERACIONES);
    std::thread t2(incrementar, ITERACIONES);

    t1.join();
    t2.join();

    std::cout << "Resultado final: " << contador << "\n";
    
    if (contador != ITERACIONES * 2) {
        std::cout << "¡ERROR! Ha ocurrido una condicion de carrera.\n";
    } else {
        std::cout << "¡Increible! Ha salido bien por pura suerte.\n";
    }
    return 0;
}
*/

