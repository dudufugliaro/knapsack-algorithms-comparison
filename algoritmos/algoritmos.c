#include "algoritmos.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>


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

/* ======================== BACKTRACKING ======================== */

void bt_recursivo(int capacidade, int pesos[], int valores[], int n, int index,
                  int valor_atual, int peso_atual, int *melhor_valor,
                  int itens_atual[], int melhores_itens[]) {
    // Caso base: chegou ao final dos itens
    if (index == n) {
        if (peso_atual <= capacidade && valor_atual > *melhor_valor) {
            *melhor_valor = valor_atual;
            // Salva a combinação de itens da melhor solução
            for (int i = 0; i < n; i++) {
                melhores_itens[i] = itens_atual[i];
            }
        }
        return;
    }

    // Inclui o item atual (se couber)
    if (peso_atual + pesos[index] <= capacidade) {
        itens_atual[index] = 1; // marca como incluído
        bt_recursivo(capacidade, pesos, valores, n, index + 1,
                     valor_atual + valores[index],
                     peso_atual + pesos[index],
                     melhor_valor, itens_atual, melhores_itens);
    }

    // Exclui o item atual
    itens_atual[index] = 0; // marca como não incluído
    bt_recursivo(capacidade, pesos, valores, n, index + 1,
                 valor_atual, peso_atual,
                 melhor_valor, itens_atual, melhores_itens);
}

int knapsack_backtracking(int capacidade, int pesos[], int valores[], int n, double* tempo_execucao) {
    clock_t inicio = clock();
    int melhor_valor = 0;

    int itens_atual[n];
    int melhores_itens[n];
    for (int i = 0; i < n; i++) {
        itens_atual[i] = 0;
        melhores_itens[i] = 0;
    }

    bt_recursivo(capacidade, pesos, valores, n, 0, 0, 0, &melhor_valor, itens_atual, melhores_itens);

    clock_t fim = clock();
    *tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Itens escolhidos (índices 0-based): ");
    for (int i = 0; i < n; i++) {
        if (melhores_itens[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return melhor_valor;
}

/* ======================== BRANCH AND BOUND ======================== */

// Função auxiliar para comparar itens pela relação valor/peso (necessária para a ordenação)
int comparar_itens(const void *a, const void *b) {
    Item *item_a = (Item *)a;
    Item *item_b = (Item *)b;
    float ratio_a = (float)item_a->valor / item_a->peso;
    float ratio_b = (float)item_b->valor / item_b->peso;
    if (ratio_a < ratio_b) return 1;
    if (ratio_a > ratio_b) return -1;
    return 0;
}

// Função para calcular o limite superior de um nó (baseado na mochila fracionária)
float calcular_limite_superior(int W, Item itens[], int n, int nivel, int peso_atual, int valor_atual) {
    float limite_superior = (float)valor_atual;
    int peso_restante = W - peso_atual;
    int i = nivel;

    while (i < n && peso_restante > 0) {
        if (itens[i].peso <= peso_restante) {
            peso_restante -= itens[i].peso;
            limite_superior += itens[i].valor;
        } else {
            limite_superior += (float)itens[i].valor * ((float)peso_restante / itens[i].peso);
            peso_restante = 0;
        }
        i++;
    }
    return limite_superior;
}

void printa_itens(Item itens[],int n){
    for(int i=0; i<n;i++){
        printf("Item %d: %d - %d\n",i,itens[i].peso,itens[i].valor);
    }
}
//converte os vetores para a estrutura itens
Item* vector_to_item(int peso[], int valor[], int n){
    Item *itens  = (Item*) malloc(sizeof(Item)*n);
    for (int i = 0; i < n; i++)
    {
        itens[i].peso = peso[i];
        itens[i].valor = valor[i];
        itens[i].i = i;
    }
    return itens;
}

//printa solucao otima
void printa_solucao(bool solucao[],int n,Item itens[]){
    for (int i = 0; i < n; i++) {
        if (solucao[itens[i].i]) {
            printf("Item %d (P: %d, V: %d)\n", i, itens[i].peso, itens[i].valor);
        }
    }
    printf("\n");
}
// Implementação do algoritmo Branch-and-Bound
int knapsack_bnb(int W, int peso[], int valor[], int n,double* tempo) {
    clock_t inicio = clock();
    Item* itens = vector_to_item(peso,valor,n);
    
    bool *solucao_otima = NULL;
    solucao_otima = (bool*)calloc(n, sizeof(bool));    
    if (solucao_otima == NULL) {
        free(itens);
        printf("Falha na alocacao da solucao.\n");
        return 1;
    }
    
    qsort(itens, n, sizeof(Item), comparar_itens);

    No *fila = NULL;
    int melhor_valor = 0;
    
    // A função é responsável por criar o nó raiz
    bool *caminho_raiz = (bool*)calloc(n, sizeof(bool));
    float limite_superior_raiz = calcular_limite_superior(W, itens, n, 0, 0, 0);
    inserir_com_prioridade(&fila, -1, 0, 0, limite_superior_raiz, caminho_raiz);
    
    while (fila != NULL) {
        No *atual = remover_da_fila(&fila);

        // Se o limite superior do nó não for promissor, pode o ramo.
        if (atual->limite_superior <= melhor_valor) {
            free(atual->caminho);
            free(atual);
            continue;
        }

        int proximo_nivel = atual->nivel + 1;

        if (proximo_nivel < n) {
            // --- RAMO ESQUERDO: Incluir o item atual (itens[proximo_nivel]) ---
            int novo_peso_esq = atual->peso_atual + itens[proximo_nivel].peso;
            int novo_valor_esq = atual->valor_atual + itens[proximo_nivel].valor;
            
            if (novo_peso_esq <= W) {
                bool *caminho_esq = (bool*)malloc(n * sizeof(bool));
                memcpy(caminho_esq, atual->caminho, n * sizeof(bool));
                caminho_esq[proximo_nivel] = true;

                float limite_superior_esq = calcular_limite_superior(W, itens, n, proximo_nivel + 1, novo_peso_esq, novo_valor_esq);

                if (novo_valor_esq > melhor_valor) {
                    melhor_valor = novo_valor_esq;
                    memcpy(solucao_otima, caminho_esq, n * sizeof(bool));
                }

                if (limite_superior_esq > melhor_valor) {
                    inserir_com_prioridade(&fila, proximo_nivel, novo_peso_esq, novo_valor_esq, limite_superior_esq, caminho_esq);
                } else {
                    free(caminho_esq); // Poda do nó
                }
            }

            // --- RAMO DIREITO: Excluir o item atual (itens[proximo_nivel]) ---
            bool *caminho_dir = (bool*)malloc(n * sizeof(bool));
            memcpy(caminho_dir, atual->caminho, n * sizeof(bool));
            caminho_dir[proximo_nivel] = false;

            float limite_superior_dir = calcular_limite_superior(W, itens, n, proximo_nivel + 1, atual->peso_atual, atual->valor_atual);

            if (limite_superior_dir > melhor_valor) {
                inserir_com_prioridade(&fila, proximo_nivel, atual->peso_atual, atual->valor_atual, limite_superior_dir, caminho_dir);
            } else {
                free(caminho_dir); // Poda do nó
            }
        } else { // Chegou a um nó folha
            if (atual->valor_atual > melhor_valor) {
                melhor_valor = atual->valor_atual;
                memcpy(solucao_otima, atual->caminho, n * sizeof(bool));
            }
        }
        
        free(atual->caminho);
        free(atual);
    }
    bool *solucao_original = (bool*)calloc(n, sizeof(bool));
    for (int i = 0; i < n; i++) {
    // 'solucao_otima' aqui se refere ao caminho da mochila com os itens ordenados
    if (solucao_otima[i]) {
        // Usa o indice_original para marcar o item na ordem correta
        int indice_original = itens[i].i;
        solucao_original[indice_original] = true;
    }
}
    printa_solucao(solucao_original,n,itens);
    free(itens);
    liberar_fila(&fila);
    free(solucao_otima);
    clock_t fim = clock();
    *tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    
    return melhor_valor;
}
