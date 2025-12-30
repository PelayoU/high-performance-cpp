#include <iostream>
#include <thread>

struct MiFuncion {
    int valor_interno;
    MiFuncion(int x) : valor_interno(x) {}

    void operator()() {
        std::cout << "Functor: Valor interno: " << valor_interno << "\n";
        valor_interno *= 2;
    }
};

int main() {
    MiFuncion f(42);
    std::thread t1(f);
    t1.join();
    std::cout << "Main: Valor original: " << f.valor_interno << "\n";
    return 0;
}
