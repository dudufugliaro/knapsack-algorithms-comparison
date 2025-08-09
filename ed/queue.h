// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definindo a estrutura para os itens da mochila, para manter a modularidade
typedef struct {
    int peso;
    int valor;
    int i;
} Item;

// Estrutura do nó para a fila de prioridade
typedef struct No {
    int nivel; // Nível na árvore, que representa o índice do item atual
    int peso_atual; // Peso acumulado dos itens já incluídos
    int valor_atual; // Valor acumulado dos itens já incluídos
    float limite_superior; // O limite superior para este nó (a prioridade)
    bool *caminho; // Rastreia quais itens foram incluídos
    struct No *proximo;
} No;


void inserir_com_prioridade(No **fila, int nivel, int peso_atual, int valor_atual, float limite_superior, bool *caminho);

No* remover_da_fila(No **fila);

void imprimir_fila(No *fila, int n);

void liberar_fila(No **fila);


#endif