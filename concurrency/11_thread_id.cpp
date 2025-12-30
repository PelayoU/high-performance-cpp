#include <iostream>
#include <thread>

void funcion_hilo() {
    std::thread::id mi_id = std::this_thread::get_id();
    std::cout << "Hijo: Mi ID es " << mi_id << "\n";
}

int main() {
    std::thread t1(funcion_hilo);
    std::thread t2(funcion_hilo);

    if (t1.get_id() != t2.get_id()) {
        std::cout << "Main: t1 y t2 son hilos distintos.\n";
    }

    std::thread t3;
    std::cout << "Main: ID de t3 (vacio): " << t3.get_id() << "\n";

    if (t3.get_id() == std::thread::id()) {
        std::cout << "Main: t3 no representa ningun hilo.\n";
    }

    t1.join();
    t2.join();
    return 0;
}
