#ifndef usuario_h
#define usuario

#include "pasta.h"

typedef struct Usuario {
    char username[30];
    char password[30];

    struct Pasta *root;

    struct Usuario *proximo;
} Usuario;

// funções públicas
Usuario* criarUtilizador(char username[], char password[]);
Usuario* login(char username[], char password[]);
void logout(Usuario **utilizadorAtual);

#endif