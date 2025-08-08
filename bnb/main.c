#include "bnb.h"
// Esta função pode ser colocada em um arquivo de utilitários (ex: utils.c)
int ler_arquivo(const char *filename, int *W, Item **itens_ptr, int *n_ptr);
int printa_itens(Item itens[],int n);
int solucao_otima_valor(const char *filename,Item itens[],int n);

int main() {
    int W, n;
    Item *itens = NULL;
    bool *solucao_otima = NULL;
    int lucro_maximo;
    
    if(!ler_arquivo("instances/ints3.txt", &W, &itens, &n)){
        printf("Falha na leitura\n");
        return 1;
    }

    solucao_otima = (bool*)calloc(n, sizeof(bool));    
    if (solucao_otima == NULL) {
        free(itens);
        printf("Falha na alocacao da solucao.\n");
        return 1;
    }
    
    lucro_maximo = branch_and_bound(W, itens, n, solucao_otima);
    printf("Lucro maximo obtido: %d\n", lucro_maximo);
    printf("\nItens na mochila (ordenados pelo algoritmo): ");
    for (int i = 0; i < n; i++) {
    // Se o valor no array for 'true' (1), o item correspondente foi selecionado
    if (solucao_otima[i]) {
        printf("\nItem %d (Peso: %d, Valor: %d) ", i, itens[i].peso, itens[i].valor);
    }
}
    return 0;
}

// Esta função pode ser colocada em um arquivo de utilitários (ex: utils.c)
int ler_arquivo(const char *filename, int *W, Item **itens_ptr, int *n_ptr) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    fscanf(file, "%d", W);

    Item *itens = NULL;
    int capacidade = 10;
    int n = 0;
    itens = (Item *)malloc(capacidade * sizeof(Item));
    if (itens == NULL) {
        fclose(file);
        return 0;
    }

    int peso, valor;
    while (fscanf(file, "%d\t%d", &peso, &valor) == 2) {
        if (n >= capacidade) {
            capacidade *= 2;
            itens = (Item *)realloc(itens, capacidade * sizeof(Item));
            if (itens == NULL) {
                fclose(file);
                return 0;
            }
        }
        itens[n].peso = peso;
        itens[n].valor = valor;
        n++;
    }
    fclose(file);
    *itens_ptr = itens;
    *n_ptr = n;
    return 1;
}
int printa_itens(Item itens[],int n){
    for (int i = 0; i < n; i++)
    {
        printf("item %d (peso-valor):\t%d\t%d\n",i,itens[i].peso,itens[i].valor);
    }
    
}


int solucao_otima_valor(const char *filename,Item itens[], int n){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }
    
    int total = 0;

    for(int i=0;i<n;i++){
        int aux = -1;
        fscanf(file,"%d", &aux);
        if (aux==1){
            total += itens[i].valor;
        }else if(aux==0) continue;
        else printf("falha na iteracao %d",i);
    }
    fclose(file);
    return total;
}