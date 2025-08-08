#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

void readFileKnapSack(const char* nome_arquivo, int* capacidade, int** valores, int** pesos, int* n) {

    FILE* file = fopen(nome_arquivo, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    // Lê a quantidade de itens
    if (fscanf(file, "%d", n) != 1) {
        printf("Erro ao ler a quantidade de itens.\n");
        fclose(file);
        exit(1);
    }

    // Lê a capacidade da mochila
    if (fscanf(file, "%d", capacidade) != 1) {
        printf("Erro ao ler a capacidade da mochila.\n");
        fclose(file);
        exit(1);
    }

    // Aloca memória para os vetores
    *valores = (int*)malloc((*n) * sizeof(int));
    *pesos   = (int*)malloc((*n) * sizeof(int));

    if (!(*valores) || !(*pesos)) {
        printf("Erro ao alocar memória.\n");
        fclose(file);
        exit(1);
    }

    // Lê os valores e pesos dos itens
    for (int i = 0; i < *n; i++) {
        if (fscanf(file, "%d %d", &(*valores)[i], &(*pesos)[i]) != 2) {
            printf("Erro ao ler valor e peso do item %d.\n", i + 1);
            fclose(file);
            exit(1);
        }
    }

    fclose(file);
}

void gerarArquivoMochila(const char* nome_arquivo, int n, int capacidade, int min_valor, int max_valor, int min_peso, int max_peso) {
    srand(time(NULL));
    
    FILE* file = fopen(nome_arquivo, "w");
    if (!file) {
        perror("Erro ao criar o arquivo");
        exit(1);
    }

    fprintf(file, "%d\n", n);
    fprintf(file, "%d\n", capacidade);
  
    for (int i = 0; i < n; i++) {
        int valor = min_valor + rand() % (max_valor - min_valor + 1);
        int peso  = min_peso  + rand() % (max_peso  - min_peso  + 1);
        fprintf(file, "%d %d\n", valor, peso);
    }

    fclose(file);
    printf("Arquivo '%s' gerado com sucesso!\n", nome_arquivo);
}

double timestamp(void) {
#ifdef _WIN32
    /* Implementação para Windows */
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / frequency.QuadPart;
#else
    /* Implementação para Linux/macOS (mantida para referência) */
    struct timespec tp;
    #ifdef __MACH__ // macOS
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    return (double)mts.tv_sec + (double)mts.tv_nsec / 1.0e9;
    #else // Linux
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (double)tp.tv_sec + (double)tp.tv_nsec / 1.0e9;
    #endif
#endif
}