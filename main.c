#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils/utils.h"
#include "algoritmos/algoritmos.h"

// gcc algoritmos.o queue.o utils.o main.o -o exe

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Uso: %s <algoritmo> <arquivo_entrada>\n", argv[0]);
        printf("Algoritmos:\n");
        printf("1 - Programação Dinâmica\n");
        printf("2 - Backtracking\n");
        printf("3 - Branch-and-Bound\n");
        return 1;
    }

    int capacidade;
    int *valores = NULL;
    int *pesos = NULL;
    int n;
    double tempo_execucao;

    readFileKnapSack(argv[2], &capacidade, &valores, &pesos, &n);

    printf("Capacidade da mochila: %d\n", capacidade);
    printf("Quantidade de itens: %d\n", n);
    
    int solucao_otima = -1;
    if (strcmp(argv[1], "1") == 0) {
        solucao_otima = knapsack_dinamica(capacidade, valores, pesos, n, &tempo_execucao);
    } else if (strcmp(argv[1], "2") == 0) {
        solucao_otima = knapsack_backtracking(capacidade, valores, pesos, n,&tempo_execucao);
    } else if (strcmp(argv[1], "3") == 0) {
        solucao_otima = knapsack_bnb(capacidade, valores, pesos, n, &tempo_execucao);
    } else {
        printf("Algoritmo inválido!\n");
        free(valores);
        free(pesos);
        return 1;
    }

    printf("Solução ótima: %d\n", solucao_otima);
    printf("Tempo de execução: %.6f segundos\n", tempo_execucao);

    free(valores);
    free(pesos);

    return 0;
}
