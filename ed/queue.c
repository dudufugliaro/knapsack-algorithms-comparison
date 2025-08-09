// queue.c
#include "queue.h"

// Insere um nó na fila de prioridade, mantendo a ordem decrescente do limite_superior
void inserir_com_prioridade(No **fila, int nivel, int peso_atual, int valor_atual, float limite_superior, bool *caminho){
    No *aux, *novo = (No*)malloc(sizeof(No));
    if(novo == NULL){
        printf("\nErro ao alocar memoria.\n");
        // Libera a memória do caminho se a alocação do nó falhar
        free(caminho);
        return;
    }

    novo->nivel = nivel;
    novo->peso_atual = peso_atual;
    novo->valor_atual = valor_atual;
    novo->limite_superior = limite_superior;
    novo->caminho = caminho; // Atribui o caminho ao novo nó
    novo->proximo = NULL;

    // A fila está vazia ou o novo nó tem a maior prioridade?
    if(*fila == NULL || novo->limite_superior > (*fila)->limite_superior){
        novo->proximo = *fila;
        *fila = novo;
    }
    else {
        // Encontra a posição correta para inserir o novo nó
        aux = *fila;
        while(aux->proximo != NULL && aux->proximo->limite_superior > novo->limite_superior){
            aux = aux->proximo;
        }
        novo->proximo = aux->proximo;
        aux->proximo = novo;
    }
}

// Remove o nó de maior prioridade (o primeiro da fila)
No* remover_da_fila(No **fila){
    No *remover = NULL;
    if(*fila != NULL){
        remover = *fila;
        *fila = remover->proximo;
    }
    else {
        // A fila está vazia, retorna NULL
    }
    return remover;
}

// Função auxiliar para imprimir a fila (útil para debug)
void imprimir_fila(No *fila, int n){
    printf("\n--- FILA DE PRIORIDADE (por Limite Superior) ---\n");
    if (fila == NULL) {
        printf("Fila vazia.\n");
        return;
    }
    while(fila){
        printf("Nivel: %d, Peso: %d, Valor: %d, Limite Sup: %.2f\n",
               fila->nivel, fila->peso_atual, fila->valor_atual, fila->limite_superior);
        printf("  Caminho: [");
        for(int i = 0; i < n; i++) {
            printf("%d ", fila->caminho[i]);
        }
        printf("]\n");
        fila = fila->proximo;
    }
    printf("--- FIM DA FILA ---\n");
}

void liberar_fila(No **fila) {
    No *no_atual = *fila;
    No *proximo_no;
    while (no_atual != NULL) {
        proximo_no = no_atual->proximo;
        if (no_atual->caminho != NULL) {
            free(no_atual->caminho);
        }
        free(no_atual);
        no_atual = proximo_no;
    }
    *fila = NULL; // Garante que o ponteiro da fila aponte para NULL no final
}