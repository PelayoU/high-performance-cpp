#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx_print;

class C {
public:
    void f(int id) {
        n++;
        n_compartida++;
        std::unique_lock<std::mutex> lock(mtx_print);
        std::cout << "Hilo " << id 
                  << " -> local_n: " << n 
                  << ", shared_n: " << n_compartida << "\n";
    }

private:
    thread_local static int n;
    static int n_compartida;
};

thread_local int C::n = 0;
int C::n_compartida = 0;

void tarea(int id) {
    C c;
    for (int i = 0; i < 3; ++i) {
        c.f(id);
    }
}

int main() {
    std::cout << "Iniciando prueba thread_local...\n";
    std::thread t1(tarea, 1);
    std::thread t2(tarea, 2);
    t1.join();
    t2.join();
    return 0;
}
