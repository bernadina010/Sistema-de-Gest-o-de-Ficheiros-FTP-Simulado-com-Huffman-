#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "pasta.h"
#include "ficheiro.h"

Pasta* criarPasta(const char *nome, Pasta *pai)
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
        printf(" - %s (%ld bytes)\n", auxF->nome, auxF->tamanho);
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
int estaDentroDeDocumentos(Pasta *pasta)
{
    while (pasta != NULL)
    {
        if (strcmp(pasta->nome, "Documentos") == 0)
            return 1;

        pasta = pasta->pai;
    }

    return 0;
}

Pasta* carregarDiretorio(const char *caminho, Pasta *pai)
{
    DIR *dir = opendir(caminho);
    if (!dir)
    {
        return NULL;
    }

    // 🔥 extrai nome do diretório de forma segura
    const char *nomeDir = strrchr(caminho, '/');
    nomeDir = nomeDir ? nomeDir + 1 : caminho;

    Pasta *atual = criarPasta(nomeDir, pai);

    // manter caminho físico correto
    strcpy(atual->caminho, caminho);

    struct dirent *entrada;

    while ((entrada = readdir(dir)) != NULL)
    {
        if (strcmp(entrada->d_name, ".") == 0 ||
            strcmp(entrada->d_name, "..") == 0)
            continue;

        char caminhoCompleto[1024];
        snprintf(caminhoCompleto, sizeof(caminhoCompleto),
                 "%s/%s", caminho, entrada->d_name);

        struct stat st;

        if (stat(caminhoCompleto, &st) == -1)
            continue;

        // =========================
        // É DIRETÓRIO
        // =========================
        if (S_ISDIR(st.st_mode))
        {
            Pasta *filho = carregarDiretorio(caminhoCompleto, atual);

            if (filho != NULL)
            {
                adicionarFilho(atual, filho);
            }
        }

        // =========================
        // É FICHEIRO
        // =========================
        else
        {
            Ficheiro *f = (Ficheiro*) malloc(sizeof(Ficheiro));

            strcpy(f->nome, entrada->d_name);
            strcpy(f->caminho, caminhoCompleto);

            // calcular tamanho real do ficheiro
            fseek(fopen(caminhoCompleto, "r"), 0, SEEK_END);
            FILE *fp = fopen(caminhoCompleto, "r");

            if (fp != NULL)
            {
                fseek(fp, 0, SEEK_END);
                f->tamanho = ftell(fp);
                fclose(fp);
            }
            else
            {
                f->tamanho = 0;
            }

            f->prox = atual->ficheiros;
            atual->ficheiros = f;
        }
    }

    closedir(dir);
    return atual;
}

Pasta* procurarPastaRecibidos(Pasta *home)
{
    if(home == NULL)
        return NULL;


    Pasta *aux = home->primeiroFilho;


    while(aux != NULL)
    {
        if(strcmp(aux->nome, "Recebidos") == 0)
        {
            return aux;
        }


        aux = aux->proximoIrmao;
    }


    return NULL;
}