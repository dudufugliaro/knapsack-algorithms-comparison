#include <stdio.h>
#include <stdlib.h>
#include "./utils/utils.h"
#include "./algoritmos/algoritmos.h"
#include <string.h>

int main(int argc, char**argv) {
    int capacidade;
    int* valores = NULL;
    int* pesos = NULL;
    int n;
    double tempo_execucao;

    //readFileKnapSack("data/test.txt", &capacidade, &valores, &pesos, &n);
    readFileKnapSack("data/test.txt",&capacidade,&valores,&pesos,&n);

    printf("Capacidade da mochila: %d\n", capacidade);

    int solucao_otima = -1;
    
    if(strcmp(argv[1],"1") == 0){
        solucao_otima = knapsack_dinamica(capacidade,valores,pesos,n,&tempo_execucao);
        printf("Solução ótima com programação dinamica : %d\n", solucao_otima);
        printf("Tempo de execução com programação dinâmica : %f (s)\n", tempo_execucao);
    }
    else if(strcmp(argv[1],"2")== 0){
        int solucao_otima = knapsack_backtracking(capacidade, valores, pesos, n, &tempo_execucao);
        printf("Solução ótima com backtracking : %d\n", solucao_otima);
        printf("Tempo de execução com backtracking : %f (s)\n", tempo_execucao);
    }
    else{
        printf("Metodo Branch and Bound selecionado\n");
        int solucao_otima = knapsack_bnb(capacidade,valores,pesos,n,&tempo_execucao);
        printf("Solução ótima com branch-and-bound : %d\n", solucao_otima);
        printf("Tempo de execução com branch-and-bound : %f (s)\n", tempo_execucao);
    }

    free(valores);
    free(pesos);

    return 0;
}
