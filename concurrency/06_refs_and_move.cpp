#include <iostream>
#include <thread>
#include <utility> // para std::ref

void modificar_valor(int& n) {
    n += 10;
    std::cout << "Hilo: Valor incrementado a " << n << "\n";
}

int main() {
    int x = 0;
    std::cout << "Main: Valor inicial x = " << x << "\n";

    std::thread t1(modificar_valor, std::ref(x));
    t1.join();

    std::cout << "Main: Valor tras t1 (esperado 10): " << x << "\n";

    std::thread t2([]{ 
        std::cout << "Hilo movido ejecutandose...\n"; 
    });

    std::thread t3;
    std::cout << "Main: Moviendo t2 a t3...\n";
    t3 = std::move(t2);

    if (!t2.joinable()) {
        std::cout << "Main: t2 ya no tiene tarea asociada (correcto).\n";
    }

    t3.join();
    return 0;
}
