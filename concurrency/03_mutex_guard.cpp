

/*
Objetivo: Solucionar el ejercicio anterior usando std::mutex y std::unique_lock.
Enunciado: Modifica el ejercicio anterior para proteger el incremento de la variable usando std::unique_lock.
Observa cómo ahora el resultado es correcto. [cite_start]Referencia: Slide 22[cite: 323].
*/

#include <iostream>
#include <thread>
#include <mutex>



int contador = 0; //Variable global, fuera del main
std::mutex m;

void incremento(){
    //std::unique_lock<std::mutex> lock {m}; Si lo pongo aqui me hara este hilo 10 mil, luego unlcok y el otro hilo 10 mil
    //Es mas rapido porque no bloqueas y desbloqueas 20 mil veces pero como contra anulas el paralelismo, los hilos no se intercalan
    for (int i=0; i<10000; i++){
        std::unique_lock<std::mutex> lock {m}; //Si lo poongo aqui bloqueo en cada iteracion (Es mas lento porque tienes
        //que bloquear y desbloquear 20 mil veces). 
        contador = contador +1;
    }

}

int main(){


    //unique lock es para no hacer lock y unlock. Al crearlo hace lock y al destruirse (por si salta excepcion o se te olvida hacer unlock)
    //pues el constructor hace unlock.
    std::thread t1 {incremento};
    std::thread t2 {incremento};

    t1.join();
    t2.join();

    std::cout << contador; //Sale 20 mil exacto, con unique_lock lo hemos solucionado entero

    return 0;

}












/*
#include <iostream>
#include <thread>
#include <mutex>

int contador = 0;
std::mutex mtx;

void incrementar_seguro(int iteraciones) {
    for (int i = 0; i < iteraciones; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        ++contador;
    }
}

int main() {
    const int ITERACIONES = 100000;
    std::cout << "Iniciando incremento seguro con Mutex...\n";

    std::thread t1(incrementar_seguro, ITERACIONES);
    std::thread t2(incrementar_seguro, ITERACIONES);

    t1.join();
    t2.join();

    std::cout << "Resultado final: " << contador << "\n";
    std::cout << "Resultado esperado: " << (ITERACIONES * 2) << "\n";
    return 0;
}

*/