#include <iostream>
#include <thread>

//Creamos una cola

std::queue<int> cola; //una cola de ints
std::mutex m;
std::condition_variable cv;

//diferencia entre notify_one y nofity_all.

void productor(){

    for (int i=0; i<100; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::unique_lock<std::mutex> l (m);
        cola.push(i);
        std::cout << "[Productor] Generado dato: " << i << "\n";
    } //al salir de esta llave se llama al destructor de unique_lock y se hace unlock

    cv.notify_one(); //notificamos al consumidor que esta dormido esperando una notificacion que hemos metido algo

}


int main(){





}


































/*
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::queue<int> cola;
std::mutex mtx;
std::condition_variable cv;
bool terminado = false;

void productor() {
    for (int i = 1; i <= 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::unique_lock<std::mutex> lock(mtx);
            cola.push(i);
            std::cout << "[Productor] Generado dato: " << i << "\n";
        }
        cv.notify_one();
    }
    {
        std::unique_lock<std::mutex> lock(mtx);
        terminado = true;
    }
    cv.notify_all();
}

void consumidor() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return !cola.empty() || terminado; });

        if (cola.empty() && terminado) {
            break;
        }

        int dato = cola.front();
        cola.pop();
        lock.unlock();
        std::cout << "\t[Consumidor] Procesando dato: " << dato << "\n";
    }
    std::cout << "\t[Consumidor] No hay mas datos. Terminando.\n";
}

int main() {
    std::thread t1(productor);
    std::thread t2(consumidor);

    t1.join();
    t2.join();
    return 0;
}

*/