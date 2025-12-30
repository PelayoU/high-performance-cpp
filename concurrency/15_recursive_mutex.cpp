#include <iostream>
#include <thread>
#include <mutex>

std::recursive_mutex r_mtx;

void funcion_recursiva(int contador) {
    if (contador <= 0) return;

    r_mtx.lock();
    std::cout << "Hilo " << std::this_thread::get_id() 
              << ": Entrando nivel " << contador << "\n";

    funcion_recursiva(contador - 1);

    std::cout << "Hilo " << std::this_thread::get_id() 
              << ": Saliendo nivel " << contador << "\n";

    r_mtx.unlock();
}

int main() {
    std::cout << "Inicio prueba recursive_mutex...\n";
    std::thread t1(funcion_recursiva, 3);
    std::thread t2(funcion_recursiva, 3);
    t1.join();
    t2.join();
    return 0;
}
