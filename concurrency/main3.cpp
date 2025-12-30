#include <iostream>
#include <chrono>
#include <thread>



void f1(int x, int y) 
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "function f1: x=" << x << " y=" << y <<std::endl;
}

struct f2 {
    f2(int px) : x{px} {}
    void operator()(int y) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "function f2: x=" << x << " y=" << y <<std::endl;
    }

    int x;
};

void f3(int x, int y) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "function f3: x=" << x << " y=" << y <<std::endl;
}

int main () {
    std::thread t1{f1, 10, 20}; 
    std::thread t2{f2{10}, 20}; 
    std::thread t3{[](int y) { f3(10,y); }, 20}; 
    t1.join();
    t2.join();
    t3.join();
}