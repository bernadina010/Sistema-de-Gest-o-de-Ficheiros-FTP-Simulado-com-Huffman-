#ifndef FICHEIRO_H
#define FICHEIRO_H

typedef struct Ficheiro {
    char nome[100];
    long tamanho;
    char caminho[200];   // caminho físico no storage

    struct Ficheiro *prox;
} Ficheiro;

// Funções do TAD
Ficheiro* criarFicheiro(char nome[], char caminho[]);
void adicionarFicheiro(Ficheiro **lista, Ficheiro *novo);
Ficheiro* procurarFicheiro(Ficheiro *lista, char nome[]);
void removerFicheiro(Ficheiro **lista, char nome[]);
void listarFicheiros(Ficheiro *lista);

#endif