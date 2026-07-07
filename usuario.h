#ifndef usuario_h
#define usuario

#include "pasta.h"

typedef struct Usuario {
    char username[30];
    char password[30];

    struct Pasta *home;

    struct Usuario *proximo;
} Usuario;

// funções públicas
Usuario* criarUtilizador(char username[], char password[]);
Usuario* login(char username[], char password[]);
void logout(Usuario **utilizadorAtual);

/*Persistência do usuário*/
void guardarUtilizador(char username[], char password[]);
void carregarUtilizadores();

int utilizadorExiste(char username[]);
void removerUtilizador(char username[], char password[]);

Usuario* procurarUsuario(char username[]);

#endif