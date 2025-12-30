#include <iostream>
#include <thread>
#include <mutex>

int contador = 0;
std::mutex mtx;

void incrementar_seguro(int iteraciones) {
    for (int i = 0; i < iteraciones; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        ++contador;
    }
}

int main() {
    const int ITERACIONES = 100000;
    std::cout << "Iniciando incremento seguro con Mutex...\n";

    std::thread t1(incrementar_seguro, ITERACIONES);
    std::thread t2(incrementar_seguro, ITERACIONES);

    t1.join();
    t2.join();

    std::cout << "Resultado final: " << contador << "\n";
    std::cout << "Resultado esperado: " << (ITERACIONES * 2) << "\n";
    return 0;
}
