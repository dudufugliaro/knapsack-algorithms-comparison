// bnb.c
#include "bnb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para a função memcpy
#include <stdbool.h>

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

// Implementação do algoritmo Branch-and-Bound
int branch_and_bound(int W, Item itens[], int n, bool solucao_otima[]) {
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
    liberar_fila(&fila);
    
    return melhor_valor;
}