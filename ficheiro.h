#ifndef FICHEIRO_H
#define FICHEIRO_H

typedef struct Ficheiro {
    char nome[100];
    long tamanho;
    char caminho[1024];   // caminho físico no storage

    struct Ficheiro *prox;
} Ficheiro;

// Funções do TAD
Ficheiro* criarFicheiro(char nome[], Pasta *diretorio, char conteudo[]);
void adicionarFicheiro(Ficheiro **lista, Ficheiro *novo);
Ficheiro* procurarFicheiro(Ficheiro *lista, char nome[]);
void removerFicheiro(Ficheiro **lista, char nome[]);
void listarFicheiros(Ficheiro *lista);

void lerFicheiro(Ficheiro *f);
void escreverFicheiro(Ficheiro *f, char conteudo[]);
void editarFicheiro(Ficheiro *f);
int ficheiroExiste(Pasta *diretorio, char nome[]);
long obterTamanhoFicheiro(FILE *fp);

#endif