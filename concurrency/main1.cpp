#include <iostream>
#include <chrono>
#include <future>



int f1(int x, int y) 
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "function f1: x=" << x << " y=" << y <<std::endl;
    return x+y;
}

struct f2 {
    f2(int px) : x{px} {}
    int operator()(int y) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "function f2: x=" << x << " y=" << y <<std::endl;
        return x+y;
    }

    int x;
};

int f3(int x, int y) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "function f3: x=" << x << " y=" << y <<std::endl;
    return x+y;
}


int main () {
    std::future<int> res_f1 = std::async(f1, 10, 20);
    std::future<int> res_f2 = std::async(f2{10}, 20);
    std::future<int> res_f3 = std::async([](int y) { return f3(10,y); }, 20);

    auto x1 = res_f1.get();
    std::cout << "resultado_f1=" << x1 << std::endl;

    auto x2 = res_f2.get();
    std::cout << "resultado_f2=" << x2 << std::endl;

    auto x3 = res_f3.get();
    std::cout << "resultado_f3=" << x3 << std::endl;

    return 0;
}