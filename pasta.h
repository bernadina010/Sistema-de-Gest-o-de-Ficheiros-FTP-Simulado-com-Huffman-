#ifndef PASTA_H
#define PASTA_H


typedef struct Ficheiro Ficheiro;

typedef struct Pasta {
    char nome[50];
    char caminho[1024];

    struct Pasta *pai;
    struct Pasta *primeiroFilho;
    struct Pasta *proximoIrmao;

    Ficheiro *ficheiros;
} Pasta;
//Funções
Pasta* criarPasta(const char *nome, Pasta *pai);

void adicionarFilho(Pasta *pai, Pasta *filho);

Pasta* procurarSubPasta(Pasta *pai, char nome[]);

void listarConteudo(Pasta *pasta);

int removerPasta(Pasta *pai, char nome[]);

int estaDentroDeDocumentos(Pasta *pasta);

Pasta* carregarDiretorio(const char *caminho, Pasta *pai);

#endif