#include <iostream>
#include <thread>
#include <vector>

void tarea_worker(int id) {
    std::cout << "Worker " << id << " trabajando...\n";
}

int main() {
    std::vector<std::thread> hilos;

    for (int i = 0; i < 5; ++i) {
        hilos.push_back(std::thread(tarea_worker, i));
    }

    for (auto& t : hilos) {
        if (t.joinable()) {
            t.join();
        }
    }
    return 0;
}
