#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"

// CRIAR NOVO NÓ

static NoHuffman* criarNo(unsigned char caractere, unsigned frequencia)
{
    NoHuffman *novo = malloc(sizeof(NoHuffman));

    if(novo == NULL)
        return NULL;

    novo->caractere = caractere;
    novo->frequencia = frequencia;

    novo->esquerda = NULL;
    novo->direita = NULL;


    return novo;
}

// CONTAR FREQUÊNCIAS

void contarFrequencias(char caminho[], unsigned frequencias[])
{

    FILE *f = fopen(caminho,"rb");

    if(f == NULL)
    {
        printf("Erro ao abrir ficheiro!\n");
        return;
    }

    for(int i=0;i<TAM_ASCII;i++)
        frequencias[i]=0;

    unsigned char c;


    while(fread(&c,sizeof(unsigned char),1,f))
    {
        frequencias[c]++;
    }
    fclose(f);

}

// HEAP MÍNIMO

static void trocar(NoHuffman **a, NoHuffman **b)
{
    NoHuffman *temp=*a;
    *a=*b;
    *b=temp;
}

static void descerHeap(HeapMinimo *heap, int indice)
{

    int menor = indice;
    int esquerda = 2*indice + 1;
    int direita = 2*indice + 2;



    if(esquerda < heap->tamanho && heap->array[esquerda]->frequencia < heap->array[menor]->frequencia)
    {
        menor = esquerda;
    }



    if(direita < heap->tamanho &&
       heap->array[direita]->frequencia < heap->array[menor]->frequencia)
    {
        menor = direita;
    }



    if(menor != indice)
    {
        trocar(
            &heap->array[menor],
            &heap->array[indice]
        );

        descerHeap(heap,menor);
    }

}

HeapMinimo* criarHeap(unsigned capacidade)
{

    HeapMinimo *heap = malloc(sizeof(HeapMinimo));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    heap->array = malloc(capacidade * sizeof(NoHuffman*));

    return heap;

}

void inserirHeap(HeapMinimo *heap,NoHuffman *no)
{
    int i = heap->tamanho;
    heap->tamanho++;

    while(i && no->frequencia < heap->array[(i-1)/2]->frequencia)
    {
        heap->array[i] = heap->array[(i-1)/2];
        i=(i-1)/2;
    }

    heap->array[i]=no;

}

NoHuffman* removerMinimo(HeapMinimo *heap)
{

    if(heap->tamanho==0)
        return NULL;

    NoHuffman *minimo = heap->array[0];
    heap->array[0] = heap->array[heap->tamanho-1];
    heap->tamanho--;

    descerHeap(heap,0);
    return minimo;
}

// CRIAR HEAP COM AS FREQUÊNCIAS

static HeapMinimo* criarHeapFrequencias(unsigned frequencias[])
{

    HeapMinimo *heap = criarHeap(TAM_ASCII);

    for(int i = 0; i < TAM_ASCII; i++)
    {
        if(frequencias[i] > 0)
        {
            NoHuffman *novo = criarNo((unsigned char)i,frequencias[i]);
            inserirHeap(heap,novo);
        }

    }


    return heap;

}


// CONSTRUIR ÁRVORE HUFFMAN
NoHuffman* construirArvoreHuffman(unsigned frequencias[])
{
    HeapMinimo *heap = criarHeapFrequencias(frequencias);

    if(heap->tamanho == 0)
        return NULL;

    while(heap->tamanho > 1)
    {
        NoHuffman *esquerda = removerMinimo(heap);
        NoHuffman *direita = removerMinimo(heap);
        NoHuffman *pai = criarNo('$',esquerda->frequencia + direita->frequencia);

        pai->esquerda = esquerda;
        pai->direita = direita;

        inserirHeap(heap,pai);
    }

    NoHuffman *raiz = removerMinimo(heap);

    free(heap->array);
    free(heap);

    return raiz;
}


// GERAR CÓDIGOS BINÁRIOS
void gerarCodigos(NoHuffman *raiz,char codigo[], int profundidade,TabelaCodigos *tabela)
{
    if(raiz == NULL)
        return;

    // É uma folha
    if(raiz->esquerda == NULL && raiz->direita == NULL)
    {
        codigo[profundidade]='\0';
        strcpy(tabela->codigo[raiz->caractere], codigo);
        return;
    }

    codigo[profundidade]='0';
    gerarCodigos(raiz->esquerda,codigo,profundidade+1,tabela);
    codigo[profundidade]='1';
    gerarCodigos(raiz->direita,codigo,profundidade+1,tabela);
}

// ESCREVER CABEÇALHO DO .HUFF
static void escreverCabecalho(FILE *saida,unsigned frequencias[])
{
    unsigned quantidade = 0;

    // contar símbolos existentes
    for(int i = 0; i < TAM_ASCII; i++)
    {
        if(frequencias[i] > 0)
            quantidade++;
    }

    // guardar quantidade de símbolos
    fwrite(&quantidade,sizeof(unsigned),1,saida);

    // guardar apenas caracteres existentes
    for(int i = 0; i < TAM_ASCII; i++)
    {
        if(frequencias[i] > 0)
        {
            unsigned char caractere = i;
            fwrite(&caractere,sizeof(unsigned char),1,saida);
            fwrite(&frequencias[i],sizeof(unsigned),1,saida);

        }
    }

}

// LER CABEÇALHO DO .HUFF
static void lerCabecalho(FILE *entrada,unsigned frequencias[])
{
    for(int i=0;i<TAM_ASCII;i++)
        frequencias[i]=0;

    unsigned quantidade;
    fread(&quantidade,sizeof(unsigned),1,entrada);

    for(unsigned i=0;i<quantidade;i++)
    {
        unsigned char caractere;
        unsigned frequencia;
        fread(&caractere,sizeof(unsigned char),1,entrada);
        fread(&frequencia,sizeof(unsigned),1,entrada);
        frequencias[caractere]=frequencia;

    }

}

// COMPRIMIR FICHEIRO
void comprimirFicheiro(char origem[],char destino[])
{

    unsigned frequencias[TAM_ASCII];
    contarFrequencias(origem,frequencias);
    NoHuffman *raiz = construirArvoreHuffman(frequencias);

    if(raiz == NULL)
    {
        printf("Erro: ficheiro vazio!\n");
        return;
    }

    TabelaCodigos tabela;
    memset(&tabela,0,sizeof(TabelaCodigos));

    char codigo[256];
    gerarCodigos(raiz,codigo,0,&tabela);

    FILE *entrada = fopen(origem,"rb");
    FILE *saida = fopen(destino,"wb");

    if(entrada == NULL ||
       saida == NULL)
    {
        printf("Erro ao abrir ficheiros!\n");
        return;
    }

    // Guardar frequências no início
    escreverCabecalho(saida,frequencias);

    unsigned char byte = 0;
    int bits = 0;
    unsigned char caractere;

    while(fread(&caractere,sizeof(unsigned char),1,entrada))
    {
        char *codigoAtual = tabela.codigo[caractere];

        for(int i=0; codigoAtual[i]!='\0' ;i++)
        {
            byte <<= 1;

            if(codigoAtual[i]=='1')
                byte |= 1;

            bits++;

            if(bits == 8)
            {
                fwrite(&byte, sizeof(unsigned char), 1, saida);
                byte=0;
                bits=0;

            }

        }

    }

    // completar último byte
    unsigned char bitsValidos = bits;
    fwrite(&bitsValidos,sizeof(unsigned char),1,saida);

    if(bits > 0)
    {
        byte <<= (8-bits);
        fwrite(&byte,sizeof(unsigned char),1,saida);
    }

    fclose(entrada);
    fclose(saida);
    libertarArvore(raiz);
    printf("Compressao concluida: %s\n",destino);

}

// LIBERTAR ÁRVORE
void libertarArvore(NoHuffman *raiz)
{
    if(raiz == NULL)
        return;

    libertarArvore(raiz->esquerda);
    libertarArvore(raiz->direita);
    free(raiz);

}

// DESCOMPRIMIR FICHEIRO
void descomprimirFicheiro(char origem[], char destino[])
{
    FILE *entrada = fopen(origem, "rb");
    FILE *saida = fopen(destino, "wb");

    if (entrada == NULL || saida == NULL)
    {
        printf("Erro ao abrir ficheiros!\n");
        return;
    }

    unsigned frequencias[TAM_ASCII];

    // Ler cabeçalho
    lerCabecalho(entrada,frequencias);

    // Reconstruir árvore
    NoHuffman *raiz = construirArvoreHuffman(frequencias);

    if (raiz == NULL)
    {
        printf("Erro ao reconstruir arvore!\n");

        fclose(entrada);
        fclose(saida);

        return;
    }

    // Ler quantidade de bits válidos do último byte
    unsigned char bitsValidos;
    fread(&bitsValidos,sizeof(unsigned char),1,entrada);
    NoHuffman *atual = raiz;
    unsigned char byte;

    // Ir até ao fim para descobrir tamanho dos dados
    long inicioDados = ftell(entrada);
    fseek(entrada,0,SEEK_END);
    long fimFicheiro = ftell(entrada);
    long tamanhoDados = fimFicheiro - inicioDados;

    // Voltar para o início dos dados comprimidos
    fseek(entrada,inicioDados,SEEK_SET);
    long contadorBytes = 0;

    while(fread(&byte,sizeof(unsigned char),1,entrada))
    {
        contadorBytes++;
        int limite = 8;


        // Se for o último byte
        if(contadorBytes == tamanhoDados)
        {
            limite = bitsValidos;
        }

        for(int i = 7; i >= 8 - limite; i--)
        {

            int bit =
                (byte >> i) & 1;

            if(bit == 0)
            {
                atual = atual->esquerda;
            }
            else
            {
                atual = atual->direita;
            }

            // Encontrou um caractere
            if(atual->esquerda == NULL &&atual->direita == NULL)
            {
                fwrite(&atual->caractere,sizeof(unsigned char),1,saida);
                atual = raiz;
            }
        }
    }

    fclose(entrada);
    fclose(saida);
    libertarArvore(raiz);
    printf("Descompressao concluida: %s\n",destino);
}