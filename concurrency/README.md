# C++ Concurrency Exercises

Ejercicios prácticos basados en programación concurrente en C++ moderno.

## Ejercicios

### 1. Creación de Hilos Básica (`01_hello_threads.cpp`) - OK
**Objetivo:** Entender cómo lanzar hilos y esperar su finalización.
**Enunciado:** Crea un programa que lance dos hilos:
1. Uno usando una función global `void f1()`.
2. Otro usando un objeto función (functor) o una lambda que reciba argumentos.
3. Asegúrate de usar `.join()` para esperar a que terminen.
[cite_start]*Referencia: Slides 12 y 16 del temario.*

### 2. Condiciones de Carrera (`02_race_condition.cpp`) - OK
**Objetivo:** Visualizar el problema de acceder a datos compartidos sin protección.
**Enunciado:** Crea una variable global `int counter = 0`. Lanza dos hilos que intenten incrementar esa variable 10,000 veces cada uno sin usar mutex. Imprime el resultado final (probablemente será incorrecto, distinto de 20,000).
[cite_start]*Referencia: Slide 15[cite: 188].*

### 3. Exclusión Mutua con RAII (`03_mutex_guard.cpp`) - OK
**Objetivo:** Solucionar el ejercicio anterior usando `std::mutex` y `std::unique_lock`.
**Enunciado:** Modifica el ejercicio anterior para proteger el incremento de la variable usando `std::unique_lock`. Observa cómo ahora el resultado es correcto.
[cite_start]*Referencia: Slide 22[cite: 323].*

### 4. Productor-Consumidor (`04_prod_cons.cpp`)
**Objetivo:** Sincronización compleja con `std::condition_variable`.
**Enunciado:** Implementa una cola segura entre hilos. Un hilo (productor) genera números y los mete en la cola; otro hilo (consumidor) espera a que haya datos y los procesa. Usa `notify_one()` y `wait()`.
[cite_start]*Referencia: Slide 27[cite: 392, 413].*

### 5. Tareas Asíncronas (`05_async_futures.cpp`)
**Objetivo:** Obtener valores de retorno de hilos.
**Enunciado:** Usa `std::async` para lanzar una tarea que calcule un valor (ej. doble de un número) y recupera el resultado usando `std::future::get()`.
[cite_start]*Referencia: Slide 33[cite: 486].*

### 6. Referencias y Movimiento (`06_refs_and_move.cpp`)
**Objetivo:** Aprender a pasar variables por referencia a un hilo y mover la propiedad de un hilo.
**Enunciado:**
1. Crea una función que modifique un valor pasado por referencia.
2. Lanza un hilo usando `std::ref()` para pasar el argumento (si no, compilará mal o hará copia).
3. Crea un hilo vacío y usa `std::move()` para transferirle la ejecución de otro hilo.
*Referencia: Slide 7 (std::ref) y Slide 9 (std::move).*

### 7. Hilos Seguros con C++20 (`07_jthread_safety.cpp`)
**Objetivo:** Ver la ventaja de `std::jthread` frente a `std::thread` en manejo de excepciones.
**Enunciado:** Implementa el ejemplo de la diapositiva 20 donde se usa `std::jthread` para asegurar que el hilo se une automáticamente al salir del ámbito, incluso si ocurre una excepción antes.
*Referencia: Slide 18 (Problema) y Slide 20 (Solución jthread).*
*(Nota: Requiere compilar con -std=c++20)*

### 8. Hilos en Segundo Plano (`08_detach_daemon.cpp`)
**Objetivo:** Crear hilos "daemon" que se ejecutan independientemente.
**Enunciado:** Lanza un hilo que imprima mensajes continuamente (simulando un servicio de fondo) y usa `.detach()` para separarlo del hilo principal.
*Referencia: Slide 21 (detach).*

### 9. Almacenamiento Local (`09_thread_local.cpp`)
**Objetivo:** Entender la diferencia entre `static` y `thread_local`.
**Enunciado:** Crea una clase con una variable `thread_local static`. Lanza varios hilos que modifiquen esa variable y comprueba que cada hilo tiene su propia copia independiente que empieza en 0.
*Referencia: Slide 27 y 32.*

### 10. Gestión de Múltiples Hilos (`10_vector_threads.cpp`)
**Objetivo:** Manejar una colección dinámica de hilos.
**Enunciado:** En lugar de crear variables `t1`, `t2`... usa un `std::vector<std::thread>` para lanzar un número indeterminado de hilos y espéralos todos con un bucle.
*Referencia: Slide 14 (Patrón vector).*

### 11. Identidad del Hilo (`11_thread_id.cpp`)
**Objetivo:** Trabajar con `std::thread::id`.
**Enunciado:** Imprime el ID del hilo principal y de los hilos hijos. Compara si dos objetos `thread` apuntan al mismo hilo y verifica si un hilo tiene ID válido antes de unirse.
*Referencia: Slides 10, 11 y 12.*

### 12. Yield y Namespaces (`12_yield_sleep.cpp`)
**Objetivo:** Usar `this_thread::yield()`.
**Enunciado:** Crea un escenario de "espera activa" donde un hilo cede el control de la CPU voluntariamente (`yield`) para no saturar el procesador mientras espera.
*Referencia: Slide 24.*


### 13. Evitando Deadlocks (`13_scoped_lock.cpp`)
**Objetivo:** Bloquear múltiples recursos a la vez de forma segura.
**Enunciado:** Simula una situación donde un hilo necesita dos recursos (mutexes) a la vez. Usa `std::scoped_lock` (C++17) para adquirirlos atómicamente y evitar el riesgo de *deadlock*.
*Referencia: Slide 16 (Adquisición de múltiples mutex).*

### 14. Functor con Estado (`14_functor_thread.cpp`)
**Objetivo:** Usar objetos función (clases que actúan como funciones).
**Enunciado:** Crea una `struct` que tenga un atributo interno (estado) y sobrecarga el `operator()`. Lanza un hilo usando una instancia de esa estructura.
*Referencia: Slide 8 (Objetos función).*

### 15. Mutex Recursivo (`15_recursive_mutex.cpp`)
**Objetivo:** Permitir que un mismo hilo adquiera el mismo cerrojo varias veces.
**Enunciado:** Implementa una función recursiva que imprime un mensaje. Intenta usar un `std::mutex` normal (que provocará deadlock) y corrígelo usando `std::recursive_mutex`.
*Referencia: Slide 8 (Exclusión mutua recursiva).*

### 16. Mutex con Tiempo (`16_timed_mutex.cpp`)
**Objetivo:** Evitar bloqueos infinitos intentando adquirir un recurso.
**Enunciado:** Usa `std::timed_mutex`. Un hilo retiene el cerrojo por un tiempo. Otro hilo intenta adquirirlo usando `try_lock_for()` con un tiempo de espera corto (falla) y luego uno largo (tiene éxito).
*Referencia: Slide 12 (Operaciones con tiempo límite).*

### 17. Lectores-Escritores (`17_shared_mutex.cpp`)
**Objetivo:** Optimizar el rendimiento permitiendo múltiples lectores simultáneos.
**Enunciado:** Usa `std::shared_mutex` (C++17). Crea una función "escritor" que adquiera bloqueo exclusivo (`lock`) y varias funciones "lector" que adquieran bloqueo compartido (`lock_shared`).
*Referencia: Slides 25 y 26 (Cerrojos compartidos).*

### 18. Bloqueo Múltiple con Defer (`18_multi_lock_defer.cpp`)
**Objetivo:** Usar `std::unique_lock` diferido y el algoritmo `std::lock`.
**Enunciado:** Declara tres mutexes y tres `unique_lock` usando la estrategia `std::defer_lock` (sin bloquear al inicio). Luego usa `std::lock(...)` para bloquearlos todos a la vez sin riesgo de deadlock.
*Referencia: Slide 23 (Algoritmos de adquisición múltiple).*

### 19. Futuros Compartidos (`19_shared_future.cpp`)
**Objetivo:** Distribuir el resultado de un hilo a múltiples hilos consumidores.
**Enunciado:** Lanza una tarea asíncrona que devuelva un valor. Usa `.share()` para convertir el futuro en un `shared_future` y pásalo a varios hilos para que todos puedan leer el resultado (el futuro normal solo permite un `.get()`).
*Referencia: Slide 39 (Compartiendo un futuro).*


# Ejercicios de Ejemplo Clase

### 20. Variantes de Lanzamiento (`20_launch_variants.cpp`)
**Fuente:** `main3.cpp` del profesor.
**Objetivo:** Dominar las tres formas de pasar argumentos a un hilo.
**Enunciado:** Lanza tres hilos que impriman "x" e "y":
1. Usando una función global `f1`.
2. Usando un objeto función (functor) `f2`.
3. Usando una expresión lambda que llame a `f3`.
*Referencia: Slide 10 (Paso de argumentos).*

### 21. Variantes Asíncronas (`21_async_return.cpp`)
**Fuente:** `main1.cpp` del profesor.
**Objetivo:** Recuperar valores de retorno usando `std::future`.
**Enunciado:** Replica el ejercicio anterior pero usando `std::async`. Las funciones deben devolver la suma de los argumentos. Recupera e imprime los resultados en el main usando `.get()`.
*Referencia: Slide 24 (Tareas asíncronas).*

### 22. Productor-Consumidor Completo (`22_prod_cons_struct.cpp`)
**Fuente:** `main2.cpp` del profesor.
**Objetivo:** Implementación canónica de monitor con variables condición.
**Enunciado:** Implementa un sistema donde:
1. Una estructura `peticion` simula carga de trabajo en su constructor (sleep).
2. Un productor genera 10 peticiones y notifica.
3. Un consumidor espera con `wait()`, extrae la petición y la procesa fuera de la sección crítica.
*Referencia: Slides 19-21.*

