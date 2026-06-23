#ifndef PASTA_H
#define PASTA_H

// Forward declaration (evita dependência circular com ficheiro.h)
typedef struct Ficheiro Ficheiro;

typedef struct Pasta {
    char nome[50];

    struct Pasta *pai;
    struct Pasta *primeiroFilho;
    struct Pasta *proximoIrmao;

    Ficheiro *ficheiros;
} Pasta;

Pasta* criarPasta(char nome[], Pasta *pai);

void adicionarFilho(Pasta *pai, Pasta *filho);

Pasta* procurarSubPasta(Pasta *pai, char nome[]);

void listarConteudo(Pasta *pasta);

#endif