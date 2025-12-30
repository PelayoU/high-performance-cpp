#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <chrono>

int proveedor_de_dato() {
    std::cout << "Proveedor: Calculando el numero magico...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 42;
}

void consumidor(std::shared_future<int> f, int id) {
    int valor = f.get();
    std::cout << "Consumidor " << id << ": Recibido " << valor << "\n";
}

int main() {
    std::future<int> fut = std::async(std::launch::async, proveedor_de_dato);
    std::shared_future<int> sh_fut = fut.share();

    std::thread t1(consumidor, sh_fut, 1);
    std::thread t2(consumidor, sh_fut, 2);
    std::thread t3(consumidor, sh_fut, 3);

    t1.join();
    t2.join();
    t3.join();
    return 0;
}
