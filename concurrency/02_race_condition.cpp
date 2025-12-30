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
