#include <iostream>
#include <thread>


void modificar(int &valor){

    valor = valor +2;

}



int main(){

    int numero=2;
    std::thread t1 {modificar, std::ref(numero)}; //para pasar una valor por referencia hay que poner std::ref

    std::thread t2;
    t2 = std::move(t1);

    t2.join();

    std::cout << numero << std::endl;



}
















/*
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
*/