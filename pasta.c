#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "pasta.h"
#include "ficheiro.h"

Pasta* criarPasta(char nome[], Pasta *pai)
{
    Pasta *nova = (Pasta*) malloc(sizeof(Pasta));

    strcpy(nova->nome, nome);

    nova->pai = pai;
    nova->primeiroFilho = NULL;
    nova->proximoIrmao = NULL;
    nova->ficheiros = NULL;

    // =========================
    // DEFINIR CAMINHO FÍSICO
    // =========================
    if (pai == NULL)
    {
        strcpy(nova->caminho, nome);
    }
    else
    {
        size_t max = sizeof(nova->caminho);

        if (snprintf(nova->caminho, max, "%s/%s", pai->caminho, nome) >= max)
        {
            printf("Erro: caminho demasiado longo!\n");
            free(nova);
            return NULL;
        }
    }

    // =========================
    // CRIAR PASTA NO DISCO
    // =========================
    mkdir(nova->caminho, 0777);

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

int removerPasta(Pasta *pai, char nome[])
{
    Pasta *atual = pai->primeiroFilho;
    Pasta *anterior = NULL;

    while (atual != NULL)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            // Não permitir remover pastas obrigatórias
            if (strcmp(nome, "Documentos") == 0 ||
                strcmp(nome, "Recebidos") == 0)
            {
                printf("Erro: esta pasta nao pode ser removida!\n");
                return 0;
            }

            // Não permitir remover pastas que contenham subpastas
            if (atual->primeiroFilho != NULL)
            {
                printf("Erro: a pasta possui subpastas!\n");
                return 0;
            }

            // Não permitir remover pastas que contenham ficheiros
            if (atual->ficheiros != NULL)
            {
                printf("Erro: a pasta possui ficheiros!\n");
                return 0;
            }

            // Remover da lista ligada
            if (anterior == NULL)
            {
                pai->primeiroFilho = atual->proximoIrmao;
            }
            else
            {
                anterior->proximoIrmao = atual->proximoIrmao;
            }

            free(atual);

            printf("Pasta removida com sucesso!\n");
            return 1;
        }

        anterior = atual;
        atual = atual->proximoIrmao;
    }

    printf("Erro: pasta nao encontrada!\n");
    return 0;
}