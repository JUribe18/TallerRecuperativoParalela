#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS 4

struct ThreadData {
    int rangoInicial;
    int rangoFinal;
    int contadorPrimos;
};

bool esPrimo(int num) {
    if (num <= 1)
        return false;

    int limite = sqrt(num);
    for (int i = 2; i <= limite; i++) {
        if (num % i == 0)
            return false;
    }

    return true;
}

void* buscarPrimos(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    data->contadorPrimos = 0;

    for (int num = data->rangoInicial; num <= data->rangoFinal; num++) {
        if (esPrimo(num)) {
            data->contadorPrimos++;
            printf("%d\n", num);
        }
    }

    pthread_exit(NULL);
}

int main() {
    printf("Buscando números primos entre 1 y 500.000.000...\n");

    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];

    int rangoInicial = 1;
    int rangoFinal = 100000000 / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].rangoInicial = rangoInicial;
        threadData[i].rangoFinal = rangoFinal;

        if (i == NUM_THREADS - 1)
            threadData[i].rangoFinal = 100000000;

        pthread_create(&threads[i], NULL, buscarPrimos, (void*)&threadData[i]);

        rangoInicial = rangoFinal + 1;
        rangoFinal += 100000000 / NUM_THREADS;
    }

    int contadorPrimosTotal = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        contadorPrimosTotal += threadData[i].contadorPrimos;
    }

    printf("Número total de primos encontrados: %d\n", contadorPrimosTotal);

    return 0;
}
