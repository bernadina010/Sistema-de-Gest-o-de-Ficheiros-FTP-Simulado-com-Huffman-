#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pasta.h"
#include "ficheiro.h"

Pasta* criarPasta(char nome[], Pasta *pai) {

    Pasta *nova = (Pasta*) malloc(sizeof(Pasta));

    strcpy(nova->nome, nome);

    nova->pai = pai;
    nova->primeiroFilho = NULL;
    nova->proximoIrmao = NULL;
    nova->ficheiros = NULL;

    return nova;
}

void adicionarFilho(Pasta *pai, Pasta *filho) {

    if (pai->primeiroFilho == NULL) {
        pai->primeiroFilho = filho;
    }
    else {
        Pasta *aux = pai->primeiroFilho;

        while (aux->proximoIrmao != NULL) {
            aux = aux->proximoIrmao;
        }

        aux->proximoIrmao = filho;
    }

    filho->pai = pai;
}

Pasta* procurarSubPasta(Pasta *pai, char nome[]) {

    Pasta *aux = pai->primeiroFilho;

    while (aux != NULL) {

        if (strcmp(aux->nome, nome) == 0) {
            return aux;
        }

        aux = aux->proximoIrmao;
    }

    return NULL;
}

void listarConteudo(Pasta *pasta) {

    printf("\n=== PASTA: %s ===\n", pasta->nome);

    // listar subpastas
    printf("\n[PASTAS]\n");

    Pasta *auxP = pasta->primeiroFilho;

    if (auxP == NULL) {
        printf(" (nenhuma)\n");
    }

    while (auxP != NULL) {
        printf(" - %s\n", auxP->nome);
        auxP = auxP->proximoIrmao;
    }

    // listar ficheiros
    printf("\n[FICHEIROS]\n");

    Ficheiro *auxF = pasta->ficheiros;

    if (auxF == NULL) {
        printf(" (nenhum)\n");
    }

    while (auxF != NULL) {
        printf(" - %s\n", auxF->nome);
        auxF = auxF->prox;
    }

    printf("\n=====================\n");
}

