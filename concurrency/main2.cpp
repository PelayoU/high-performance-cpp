#include <iostream>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>


struct peticion {
    peticion(int px): x{px} {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "genera peticion: x=" << x <<std::endl;
    }
    void procesar() {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "procesar peticion: x=" << x <<std::endl;
    }
    int x;
};

std::queue<peticion> cola; // Cola de peticiones
std::condition_variable cv; // Variable condición
std::mutex m;

void productor() {
    for (int i=0; i<10; i++) {
        peticion p = peticion{i};
        std::unique_lock l{m};
        cola.push(p);
        cv.notify_one();
    }
}

void consumidor() {
    for (int i=0; i<10; i++) {
        std::unique_lock l{m};
        while (cola.empty()) {
            cv.wait(l);
        }
        auto p = cola.front();
        cola.pop();
        l.unlock();
        p.procesar();
    }
}


int main () {
    std::thread t1{productor}; 
    std::thread t2{consumidor}; 
    t1.join();
    t2.join();
}