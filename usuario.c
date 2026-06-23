#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usuario.h"

// lista global de utilizadores
static Usuario *listaUsuarios = NULL;

Usuario* criarUtilizador(char username[], char password[]) {

    Usuario *novo = (Usuario*) malloc(sizeof(Usuario));

    strcpy(novo->username, username);
    strcpy(novo->password, password);

    novo->root = criarPasta("root", NULL);

    // pastas obrigatórias
    Pasta *doc = criarPasta("Documentos", novo->root);
    Pasta *rec = criarPasta("Recebidos", novo->root);

    adicionarFilho(novo->root, doc);
    adicionarFilho(novo->root, rec);

    novo->proximo = listaUsuarios;
    listaUsuarios = novo;

    printf("Utilizador criado com sucesso!\n");

    return novo;
}

Usuario* login(char username[], char password[]) {

    Usuario *aux = listaUsuarios;

    while (aux != NULL) {

        if (strcmp(aux->username, username) == 0 &&
            strcmp(aux->password, password) == 0) {

            printf("Login efetuado com sucesso!\n");
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