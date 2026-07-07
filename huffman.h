#ifndef HUFFMAN_H
#define HUFFMAN_H

#define TAM_ASCII 256


typedef struct NoHuffman
{
    unsigned char caractere;
    unsigned frequencia;

    struct NoHuffman *esquerda;
    struct NoHuffman *direita;

} NoHuffman;


typedef struct
{
    unsigned tamanho;
    unsigned capacidade;
    NoHuffman **array;

} HeapMinimo;

typedef struct
{
    char codigo[TAM_ASCII][256];

} TabelaCodigos;

typedef struct
{
    unsigned char caractere;
    unsigned frequencia;
} SimboloHuffman;

// Frequências
void contarFrequencias(char caminho[],unsigned frequencias[]);

// Heap
HeapMinimo* criarHeap(unsigned capacidade);

void inserirHeap(HeapMinimo *heap,NoHuffman *no);

NoHuffman* removerMinimo(HeapMinimo *heap);

// Árvore Huffman
NoHuffman* construirArvoreHuffman(unsigned frequencias[]);

// Códigos
void gerarCodigos(NoHuffman *raiz,char codigo[],int profundidade,TabelaCodigos *tabela);

// Compressão
void comprimirFicheiro(char origem[], char destino[]);

// Descompressão
void descomprimirFicheiro(char origem[], char destino[]);

// Libertação memória
void libertarArvore(NoHuffman *raiz);

#endif