#include <iostream>
#include <thread>
#include <chrono>

void actualiza_reloj() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "[Background] Actualizando reloj...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "[Background] Fin del hilo demonio.\n";
}

void funcion_con_detach() {
    std::cout << "Main: Lanzando hilo demonio...\n";
    std::thread t(actualiza_reloj);
    t.detach();
    std::cout << "Main: Hilo detached. La funcion termina pero el hilo sigue.\n";
}

int main() {
    funcion_con_detach();
    std::cout << "Main: Esperando 2 segundos simulando trabajo...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Main: Fin del programa.\n";
    return 0;
}
