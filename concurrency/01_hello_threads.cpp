#include <iostream>
#include <thread>
#include <string>

// Función simple que ejecutará un hilo
void saludar() {
    std::cout << "Hilo 1: Hola desde una funcion void!\n";
}

// Función que recibe argumentos (copia por valor)
void saludar_con_args(int id, std::string mensaje) {
    std::cout << "Hilo 2 (ID " << id << "): " << mensaje << "\n";
}

int main() {
    std::cout << "Main: Lanzando hilos...\n";

    // 1. Hilo básico
    std::thread t1(saludar);

    // 2. Hilo con argumentos
    std::thread t2(saludar_con_args, 42, "Argumentos pasados correctamente");

    // 3. Hilo usando una expresión Lambda
    std::thread t3([]{
        std::cout << "Hilo 3: Hola desde una lambda!\n";
    });

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Main: Todos los hilos terminaron correctamente.\n";
    return 0;
}
