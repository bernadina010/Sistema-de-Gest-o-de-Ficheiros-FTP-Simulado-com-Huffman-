#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pasta.h"
#include "ficheiro.h"

Ficheiro* criarFicheiro(char nome[], Pasta *diretorio, char conteudo[])
{
    // =========================
    // CRIAR NÓ LÓGICO
    // =========================
    Ficheiro *novo = (Ficheiro*) malloc(sizeof(Ficheiro));

    strcpy(novo->nome, nome);

    size_t max = sizeof(novo->caminho);

    if (snprintf(novo->caminho, max, "%s/%s", diretorio->caminho, nome) >= max)
    {
        printf("Erro: caminho do ficheiro demasiado longo!\n");
        free(novo);
        return NULL;
    }

    novo->prox = NULL;

    // =========================
    // CRIAR FICHEIRO FÍSICO
    // =========================
    FILE *fp = fopen(novo->caminho, "w");

    if (fp == NULL)
    {
        printf("Erro ao criar ficheiro no disco!\n");
        free(novo);
        return NULL;
    }

    // escrever conteúdo inicial
    if (conteudo != NULL)
    {
        fprintf(fp, "%s", conteudo);
    }

    fclose(fp);

    printf("Ficheiro criado em: %s\n", novo->caminho);

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