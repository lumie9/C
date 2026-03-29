#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define QUANT_THREADS 8

pthread_barrier_t barreira;
pthread_mutex_t mutex_vencedor = PTHREAD_MUTEX_INITIALIZER;

char vencedor_nome[50] = "";
int alguem_ganhou = 0;

void *func_thread(void *arg) {
    char* nome = (char*)arg;

    pthread_barrier_wait(&barreira);

    for (int i = 0; i < 100; i++) {
        printf("%s: %d \n", nome, i);
    }

    // linha de chegada
    pthread_mutex_lock(&mutex_vencedor);
    if (alguem_ganhou == 0) {
        strcpy(vencedor_nome, nome);
        alguem_ganhou = 1;
    }
    pthread_mutex_unlock(&mutex_vencedor);

    printf("%s terminou!\n", nome);
    
    free(nome); 
    return NULL;
}

int main() {
    pthread_t threads[QUANT_THREADS];
    pthread_barrier_init(&barreira, NULL, QUANT_THREADS);

    for (int i = 0; i < QUANT_THREADS; i++) {
        char* id_nome = malloc(16 * sizeof(char));
        sprintf(id_nome, "Thread %d", i + 1);
        pthread_create(&threads[i], NULL, func_thread, (void*)id_nome);
    }

    // espera todas threads terminarem
    for (int i = 0; i < QUANT_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barreira);
    printf("\nVencedor: %s\n", vencedor_nome);
    return 0;
}