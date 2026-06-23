#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ficheiro.h"

Ficheiro* criarFicheiro(char nome[], char caminho[]) {

    Ficheiro *novo = (Ficheiro*) malloc(sizeof(Ficheiro));

    strcpy(novo->nome, nome);
    strcpy(novo->caminho, caminho);

    novo->tamanho = 0;
    novo->prox = NULL;

    return novo;
}

void adicionarFicheiro(Ficheiro **lista, Ficheiro *novo) {

    if (*lista == NULL) {
        *lista = novo;
    }
    else {
        Ficheiro *aux = *lista;

        while (aux->prox != NULL) {
            aux = aux->prox;
        }

        aux->prox = novo;
    }
}

Ficheiro* procurarFicheiro(Ficheiro *lista, char nome[]) {

    Ficheiro *aux = lista;

    while (aux != NULL) {

        if (strcmp(aux->nome, nome) == 0) {
            return aux;
        }

        aux = aux->prox;
    }

    return NULL;
}

void removerFicheiro(Ficheiro **lista, char nome[]) {

    Ficheiro *aux = *lista;
    Ficheiro *anterior = NULL;

    while (aux != NULL) {

        if (strcmp(aux->nome, nome) == 0) {

            if (anterior == NULL) {
                *lista = aux->prox;
            }
            else {
                anterior->prox = aux->prox;
            }

            free(aux);
            printf("Ficheiro removido com sucesso!\n");
            return;
        }

        anterior = aux;
        aux = aux->prox;
    }

    printf("Ficheiro nao encontrado!\n");
}

void listarFicheiros(Ficheiro *lista) {

    printf("\n=== FICHEIROS ===\n");

    if (lista == NULL) {
        printf(" (nenhum ficheiro)\n");
        return;
    }

    Ficheiro *aux = lista;

    while (aux != NULL) {
        printf(" - %s | %ld bytes\n", aux->nome, aux->tamanho);
        aux = aux->prox;
    }

    printf("=================\n");
}