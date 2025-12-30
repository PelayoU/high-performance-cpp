#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::timed_mutex t_mtx;

void tarea_larga() {
    t_mtx.lock();
    std::cout << "[Tarea Larga] Mutex adquirido. Durmiendo 2s...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    t_mtx.unlock();
    std::cout << "[Tarea Larga] Mutex liberado.\n";
}

void intentar_entrar() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (t_mtx.try_lock_for(std::chrono::milliseconds(500))) {
        std::cout << "[Intento 1] Exito!\n";
        t_mtx.unlock();
    } else {
        std::cout << "[Intento 1] Fallo: Tiempo agotado.\n";
    }

    if (t_mtx.try_lock_for(std::chrono::seconds(3))) {
        std::cout << "[Intento 2] Exito! Adquirido tras esperar.\n";
        t_mtx.unlock();
    } else {
        std::cout << "[Intento 2] Fallo.\n";
    }
}

int main() {
    std::thread t1(tarea_larga);
    std::thread t2(intentar_entrar);
    t1.join();
    t2.join();
    return 0;
}
