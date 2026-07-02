#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "usuario.h"


// lista global de utilizadores
static Usuario *listaUsuarios = NULL;

int utilizadorExiste(char username[])
{
    Usuario *aux = listaUsuarios;

    while(aux != NULL)
    {
        if(strcmp(aux->username, username) == 0)
        {
            return 1;
        }

        aux = aux->proximo;
    }

    return 0;
}

Usuario* criarUtilizador(char username[], char password[]) {
    
    printf("Verificando existencia de %s...\n", username);
    if(utilizadorExiste(username))
    {
        printf("Erro: utilizador ja existe!\n");
        return NULL;
    }

    Usuario *novo = (Usuario*) malloc(sizeof(Usuario));

    if(novo != NULL)
    {
        printf("Utilizador criado com sucesso!\n");
    }

    strcpy(novo->username, username);
    strcpy(novo->password, password);

    /* CRIAR PASTAS FÍSICAS */

    char caminho[200];

    mkdir(username, 0777);

    sprintf(caminho, "%s/Documentos", username);
    mkdir(caminho, 0777);

    sprintf(caminho, "%s/Recebidos", username);
    mkdir(caminho, 0777);

    /* ÁRVORE LÓGICA */

    novo->home= criarPasta(username, NULL);

    // pastas obrigatórias
    Pasta *doc = criarPasta("Documentos", novo->home);
    Pasta *rec = criarPasta("Recebidos", novo->home);

    adicionarFilho(novo->home, doc);
    adicionarFilho(novo->home, rec);

    novo->proximo = listaUsuarios;
    listaUsuarios = novo;

    guardarUtilizador(username, password);

    printf("Utilizador criado com sucesso!\n");

    return novo;
}

Usuario* login(char username[], char password[])
{
    Usuario *aux = listaUsuarios;

    while (aux != NULL)
    {
        if (strcmp(aux->username, username) == 0 &&
            strcmp(aux->password, password) == 0)
        {
            printf("Login efetuado com sucesso!\n");

            // 🔥 AQUI É O PONTO CRÍTICO
            char caminhoBase[200];

            snprintf(caminhoBase, sizeof(caminhoBase),
                     "%s", aux->username);

            aux->home = carregarDiretorio(caminhoBase, NULL);

            return aux;
        }

        aux = aux->proximo;
    }

    printf("Erro: login invalido!\n");
    return NULL;
}
void logout(Usuario **utilizadorAtual) {

    *utilizadorAtual = NULL;
    printf("Logout efetuado com sucesso!\n");
}

void guardarUtilizador(char username[], char password[])
{
    FILE *fp = fopen("utilizadores.txt", "a");

    if(fp == NULL)
    {
        printf("Erro ao abrir utilizadores.txt\n");
        return;
    }

    fprintf(fp, "%s;%s\n", username, password);

    fclose(fp);
}

static Usuario* criarUtilizadorMemoria(char username[], char password[])
{
    Usuario *novo = (Usuario*) malloc(sizeof(Usuario));

    strcpy(novo->username, username);
    strcpy(novo->password, password);

    /* recriar árvore lógica */

    novo->home = criarPasta(username, NULL);

    Pasta *doc = criarPasta("Documentos", novo->home);
    Pasta *rec = criarPasta("Recebidos", novo->home);

    adicionarFilho(novo->home, doc);
    adicionarFilho(novo->home, rec);

    novo->proximo = listaUsuarios;
    listaUsuarios = novo;

    return novo;
}

void carregarUtilizadores()
{
    FILE *fp = fopen("utilizadores.txt", "r");

    if(fp == NULL)
    {
        return;
    }

    char linha[100];

    while(fgets(linha, sizeof(linha), fp) != NULL)
    {
        char username[30];
        char password[30];

        linha[strcspn(linha, "\n")] = '\0';

        if(sscanf(linha, "%29[^;];%29s",
                  username,
                  password) == 2)
        {
            criarUtilizadorMemoria(username, password);
        }
    }

    fclose(fp);
}

void removerUtilizador(char username[], char password[])
{
    Usuario *atual = listaUsuarios;
    Usuario *anterior = NULL;

    while(atual != NULL)
    {
        if(strcmp(atual->username, username) == 0 &&
           strcmp(atual->password, password) == 0)
        {
            /* remover da lista ligada */

            if(anterior == NULL)
            {
                listaUsuarios = atual->proximo;
            }
            else
            {
                anterior->proximo = atual->proximo;
            }

            free(atual);

            printf("Utilizador removido da memoria.\n");

            /* reescrever utilizadores.txt */

            FILE *fp = fopen("utilizadores.txt", "w");

            if(fp == NULL)
            {
                printf("Erro ao atualizar utilizadores.txt\n");
                return;
            }

            Usuario *aux = listaUsuarios;

            while(aux != NULL)
            {
                fprintf(fp,
                        "%s;%s\n",
                        aux->username,
                        aux->password);

                aux = aux->proximo;
            }

            fclose(fp);

            //Remove pasta fisica do disco
            char comando[300];
            sprintf(comando, "rm -rf %s", username);
            system(comando);

            printf("Utilizador removido com sucesso!\n");

            return;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    printf("Utilizador nao encontrado!\n");
}