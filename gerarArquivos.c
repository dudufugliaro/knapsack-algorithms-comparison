#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h> // Para _mkdir no Windows
#include "utils/utils.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <n> <W> <num_instancias>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int capacidade = atoi(argv[2]);
    int num_instancias = atoi(argv[3]);
    int min_valor = 1, max_valor = 1000;
    int min_peso = 1, max_peso = n / 4;

    char dir_name[100];
    sprintf(dir_name, "instances\\n%d_W%d", n, capacidade);
    
    _mkdir("instances"); // Cria o diretório principal
    _mkdir(dir_name);    // Cria o subdiretório

    for (int i = 0; i < num_instancias; i++) {
        char filename[150];
        sprintf(filename, "%s\\instance_%d.txt", dir_name, i+1);
        gerarArquivoMochila(filename, n, capacidade, min_valor, max_valor, min_peso, max_peso);
    }

    return 0;
}