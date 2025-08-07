#include "algoritmos.h"
#include <stdio.h>
#include <time.h>

int knapsack_dinamica(int W, int w[], int v[], int n,double* tempo_execucao) {

    // essa matriz representa o valor máximo que conseguimos com os primeiros i itens e com
    // a capacidade da mochila igual a j
    int dp[n + 1][W + 1];
    // a matriz vai de 0 a n itens e de 0 a W capacidades

    // inicializa a primeira linha e coluna com 0
    // para i=0 (0 itens) nada pode ser colocado na mochila (0)
    // para j=0 (0 capacidade) nada cabe na mochila (0)
    clock_t inicio = clock();

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= W; j++)
            dp[i][j] = 0;


    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= W; j++) {

            //verfica se o item (i-1) cabe na mochila com j de espaço
            if (w[i - 1] <= j) {
                
                //se incluir, o valor total será o valor do item i-1 + o melhor valor com os itens anteriores
                int inclui = v[i - 1] + dp[i - 1][j - w[i - 1]];
                //se não incluir, o valor total será o mesmo de antes
                int exclui = dp[i - 1][j];
                //escolhe a melhor opçãp
                dp[i][j] = (inclui > exclui) ? inclui : exclui;
            } else {
                // item não cabe na mochila
                dp[i][j] = dp[i - 1][j];
            }

        }
    }

    // Imprime os itens selecionados 
    printf("Itens escolhidos (índices 0-based): ");
    int i = n, j = W;
    while (i > 0 && j > 0) {
        if (dp[i][j] != dp[i - 1][j]) {
            printf("%d ", i - 1);  // item i-1 foi incluído
            j -= w[i - 1];         // reduz capacidade
        }
        i--;
    }
    printf("\n");

    clock_t fim = clock();
    *tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    return dp[n][W];  
}