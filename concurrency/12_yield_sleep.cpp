#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

std::atomic<bool> listo(false);

void esperar_activamente() {
    while (!listo) {
        std::this_thread::yield();
    }
    std::cout << "Hijo: Ya estoy listo!\n";
}

int main() {
    std::thread t(esperar_activamente);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    listo = true;
    t.join();
    return 0;
}
