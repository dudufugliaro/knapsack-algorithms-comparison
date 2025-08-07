#include "./utils/utils.h"
#include <time.h>

int main() {

   
    int n = 1000;                 
    int capacidade = 100;        
    int min_valor = 1, max_valor = 1000;
    int min_peso = 1, max_peso = n / 4;

    gerarArquivoMochila("data/n=1000&W=100/mochila_1000_100_10.txt", n, capacidade, min_valor, max_valor, min_peso, max_peso);

    return 0;
}