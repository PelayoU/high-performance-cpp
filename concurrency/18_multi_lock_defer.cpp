#include <iostream>
#include <thread>
#include <mutex>

std::mutex m1, m2, m3;

void operacion_segura() {
    std::unique_lock<std::mutex> l1(m1, std::defer_lock);
    std::unique_lock<std::mutex> l2(m2, std::defer_lock);
    std::unique_lock<std::mutex> l3(m3, std::defer_lock);

    std::lock(l1, l2, l3);

    std::cout << "Hilo " << std::this_thread::get_id() 
              << ": Tengo m1, m2 y m3.\n";
}

int main() {
    std::thread t1(operacion_segura);
    std::thread t2(operacion_segura);
    t1.join();
    t2.join();
    return 0;
}
