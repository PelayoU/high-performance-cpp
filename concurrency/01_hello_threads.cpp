#include<iostream>
#include<thread>
#include<string>


void f1(){
    std::cout<< 2+2<<std::endl;
}

struct Saludo{
    std::string comienzo;

    Saludo(std::string comien): comienzo(comien) {}

    void operator()(std::string nombre){
        std::cout << comienzo << " " << nombre<<std::endl;
    }

};


/*
Para pasar el objeto funcion a lambda se ponen entre [] el constructor, y luego
entre parentesis el argumento del metodo operator y luego entre llaves el contendio
del metodo operator

std::string comien = "Hola";

// Lambda que captura 'comien' y recibe 'nombre' como argumento
auto saludo_lambda = [comien](std::string nombre) {
    std::cout << comien << " " << nombre;
};


*/
int main(){

    std::thread t1 {f1};
    std::thread t2 {Saludo("Hola Buenas"), "Pelayo"};
    //Lo mismo con lambda.
    std::string comien = "Hola";
    //std::string nombre_fijo = "Pelayo"; si no ponemos esto se lo tienes que pasar al final
    std::thread t3 {[comien](std::string nombre){std::cout << comien << " " << nombre<<std::endl;}, "Pelayo"};

    t1.join();
    t2.join();
    t3.join();

    return 0;



}






















/*

#include <iostream>
#include <thread>
#include <string>

// Función simple que ejecutará un hilo
void saludar() {
    std::cout << "Hilo 1: Hola desde una funcion void!\n";
}

// Función que recibe argumentos (copia por valor)
void saludar_con_args(int id, std::string mensaje) {
    std::cout << "Hilo 2 (ID " << id << "): " << mensaje << "\n";
}

int main() {
    std::cout << "Main: Lanzando hilos...\n";

    // 1. Hilo básico
    std::thread t1(saludar);

    // 2. Hilo con argumentos
    std::thread t2(saludar_con_args, 42, "Argumentos pasados correctamente");

    // 3. Hilo usando una expresión Lambda
    std::thread t3([]{
        std::cout << "Hilo 3: Hola desde una lambda!\n";
    });

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Main: Todos los hilos terminaron correctamente.\n";
    return 0;
}
*/
