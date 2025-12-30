#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <chrono>

std::shared_mutex smtx;
int dato_compartido = 0;

void escritor(int valor) {
    std::unique_lock<std::shared_mutex> lock(smtx);
    std::cout << "[Escritor] Escribiendo " << valor << "...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    dato_compartido = valor;
}

void lector(int id) {
    std::shared_lock<std::shared_mutex> lock(smtx);
    std::cout << "\t[Lector " << id << "] Leyendo: " << dato_compartido << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main() {
    std::vector<std::thread> hilos;

    for(int i = 0; i < 3; ++i) {
        hilos.emplace_back(escritor, i + 1);
        hilos.emplace_back(lector, i * 10);
        hilos.emplace_back(lector, i * 10 + 1);
    }

    for (auto& t : hilos) t.join();
    return 0;
}
