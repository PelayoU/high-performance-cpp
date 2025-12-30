#include <iostream>
#include <thread>

void tarea() {
    std::cout << "Tarea: Ejecutando...\n";
}

void ejemplo_jthread() {
    std::cout << "--- Inicio ejemplo jthread (Slide 20) ---\n";
    std::jthread t1(tarea);

    if (true) {
        std::cout << "Main: Saliendo del scope prematuramente...\n";
        return;
    }
}

int main() {
#if __cplusplus >= 202002L
    ejemplo_jthread();
    std::cout << "--- Fin ejemplo jthread ---\n";
#else
    std::cout << "Necesitas compilar con -std=c++20 para probar jthread.\n";
#endif
    return 0;
}
