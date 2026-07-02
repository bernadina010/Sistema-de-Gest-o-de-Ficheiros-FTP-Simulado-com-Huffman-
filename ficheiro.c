#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pasta.h"
#include "ficheiro.h"

int ficheiroExiste(Pasta *diretorio, char nome[])
{
    Ficheiro *aux = diretorio->ficheiros;

    while (aux != NULL)
    {
        if (strcmp(aux->nome, nome) == 0)
        {
            return 1;
        }

        aux = aux->prox;
    }

    return 0;
}

Ficheiro* criarFicheiro(char nome[], Pasta *diretorio, char conteudo[])
{   
    if (ficheiroExiste(diretorio, nome))
    {
        printf("Erro: já existe um ficheiro com esse nome nesta pasta!\n");
        return NULL;
    }
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
    fflush(fp);
    novo->tamanho = obterTamanhoFicheiro(fp);

    fclose(fp);

    printf("Ficheiro criado em: %s\n", novo->caminho);

    return novo;
}

long obterTamanhoFicheiro(FILE *fp)
{
    long tamanho;

    fseek(fp, 0, SEEK_END);
    tamanho = ftell(fp);
    rewind(fp);

    return tamanho;
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

            if (remove(aux->caminho) != 0)
            {
                printf("Aviso: nao foi possivel remover o ficheiro do disco.\n");
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

void lerFicheiro(Ficheiro *f)
{
    if (f == NULL)
    {
        printf("Ficheiro invalido!\n");
        return;
    }

    FILE *fp = fopen(f->caminho, "r");

    if (fp == NULL)
    {
        printf("Erro ao abrir ficheiro!\n");
        return;
    }

    char linha[256];

    printf("\n--- CONTEUDO ATUAL ---\n");

    while (fgets(linha, sizeof(linha), fp))
    {
        printf("%s", linha);
    }

    printf("\n----------------------\n");

    fclose(fp);
}

void escreverFicheiro(Ficheiro *f, char conteudo[])
{
    if (f == NULL)
    {
        printf("Ficheiro invalido!\n");
        return;
    }

    FILE *fp = fopen(f->caminho, "w");

    if (fp == NULL)
    {
        printf("Erro ao escrever no ficheiro!\n");
        return;
    }

    fprintf(fp, "%s", conteudo);

    fflush(fp);
    fclose(fp);

    FILE *fp2 = fopen(f->caminho, "r");

    if (fp2 != NULL)
    {
        f->tamanho = obterTamanhoFicheiro(fp2);
        fclose(fp2);
    }
}

void editarFicheiro(Ficheiro *f)
{
    if (f == NULL)
    {
        printf("Ficheiro invalido!\n");
        return;
    }

    lerFicheiro(f);

    char novoConteudo[1024];

    printf("\nNovo conteudo (substitui o anterior):\n");

    getchar();
    fgets(novoConteudo, sizeof(novoConteudo), stdin);

    novoConteudo[strcspn(novoConteudo, "\n")] = '\0';

    escreverFicheiro(f, novoConteudo);

    FILE *fp = fopen(f->caminho, "r");

    if (fp != NULL)
    {
        f->tamanho = obterTamanhoFicheiro(fp);
        fclose(fp);
    }

    printf("Ficheiro atualizado com sucesso!\n");
}

