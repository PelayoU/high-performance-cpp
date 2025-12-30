#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m1;
std::mutex m2;

void operacion_compleja_segura() {
    std::scoped_lock lock(m1, m2);
    std::cout << "Hilo " << std::this_thread::get_id() << " trabajando...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    std::thread t1(operacion_compleja_segura);
    std::thread t2(operacion_compleja_segura);
    t1.join();
    t2.join();
    return 0;
}
