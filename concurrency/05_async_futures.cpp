#include <iostream>
#include <future>
#include <thread>
#include <chrono>

double calculo_pesado(double base, double potencia) {
    std::cout << "Tarea: Iniciando calculo en segundo plano...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    double resultado = 1.0;
    for(int i=0; i<(int)potencia; ++i) resultado *= base;
    
    std::cout << "Tarea: Calculo finalizado.\n";
    return resultado;
}

int main() {
    std::cout << "Main: Iniciando programa.\n";

    std::future<double> ticket_resultado =
        std::async(std::launch::async, calculo_pesado, 2.0, 10.0);

    std::cout << "Main: El hilo principal sigue trabajando mientras la tarea corre...\n";
    for(int i=0; i<5; ++i) {
        std::cout << ".";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "\n";

    double valor = ticket_resultado.get(); 

    std::cout << "Main: Resultado recibido de la tarea asincrona: " << valor << "\n";
    return 0;
}
