#include <stdio.h>
#include <stdlib.h>

#include "usuario.h"
#include "pasta.h"
#include "ficheiro.h"

int main()
{

    Usuario *utilizadorAtual = NULL;
    Pasta *diretorioAtual = NULL;

    int opcao;

    do
    {

        // ================= MENU SEM LOGIN =================
        if (utilizadorAtual == NULL)
        {

            printf("\n======================================\n");
            printf("   SISTEMA DE GESTAO DE FICHEIROS\n");
            printf("======================================\n");
            printf("Estado: SEM UTILIZADOR LOGADO\n\n");

            printf("1. Criar utilizador\n");
            printf("2. Login\n");
            printf("3. Sair\n");
            printf("--------------------------------------\n");
            printf("Opcao: ");
            scanf("%d", &opcao);

            switch (opcao)
            {

            case 1:
            {
                char user[30], pass[30];

                printf("Username: ");
                scanf("%s", user);

                printf("Password: ");
                scanf("%s", pass);

                criarUtilizador(user, pass);
            }
            break;

            case 2:
            {
                char user[30], pass[30];

                printf("Username: ");
                scanf("%s", user);

                printf("Password: ");
                scanf("%s", pass);

                utilizadorAtual = login(user, pass);
            }
            break;

            case 3:
                printf("A encerrar sistema...\n");
                exit(0);

            default:
                printf("Opcao invalida!\n");
            }
        }

        // ================= MENU COM LOGIN =================
        else
        {
            if (diretorioAtual == NULL)
            {
                diretorioAtual = utilizadorAtual->root;
            }

            printf("\n======================================\n");
            printf(" UTILIZADOR: %s\n", utilizadorAtual->username);
            printf(" DIRETORIO: %s\n", diretorioAtual->nome);
            printf("======================================\n");

            printf("1. Listar conteudo\n");
            printf("2. Criar ficheiro\n");
            printf("3. Criar pasta\n");
            printf("4. Entrar em pasta\n");
            printf("5. Voltar pasta anterior\n");
            printf("6. Abrir ficheiro\n");
            printf("7. Eliminar ficheiro\n");
            printf("8. Logout\n");
            printf("9. Sair\n");
            printf("--------------------------------------\n");
            printf("Opcao: ");
            scanf("%d", &opcao);

            switch (opcao)
            {

            case 1:
                if (diretorioAtual == NULL)
                {
                    printf("Erro: nenhum diretorio ativo!\n");
                    break;
                }
                listarConteudo(diretorioAtual);
                break;

            case 2:
            {
                char nome[100];

                printf("Nome do ficheiro: ");
                scanf("%s", nome);

                // criar caminho fictício (simulação storage)
                char caminho[200];
                sprintf(caminho, "storage/%s/%s/%s",
                        utilizadorAtual->username,
                        diretorioAtual->nome,
                        nome);

                Ficheiro *novo = criarFicheiro(nome, caminho);

                adicionarFicheiro(&diretorioAtual->ficheiros, novo);

                printf("Ficheiro criado!\n");
            }
            break;

            case 3:
                if (diretorioAtual == NULL)
                {
                    printf("Erro: nenhum diretorio ativo!\n");
                    break;
                }
                {
                    char nome[50];

                    printf("Nome da pasta: ");
                    scanf("%s", nome);

                    Pasta *nova = criarPasta(nome, diretorioAtual);

                    adicionarFilho(diretorioAtual, nova);

                    printf("Pasta criada!\n");
                }
                break;

            case 4:
                if (diretorioAtual == NULL)
                {
                    printf("Erro: nenhum diretorio ativo!\n");
                    break;
                }
                {
                    char nome[50];

                    printf("Nome da pasta: ");
                    scanf("%s", nome);

                    Pasta *destino = procurarSubPasta(diretorioAtual, nome);

                    if (destino != NULL)
                    {
                        diretorioAtual = destino;
                        printf("Entrou na pasta %s\n", nome);
                    }
                    else
                    {
                        printf("Pasta nao encontrada!\n");
                    }
                }
                break;

            case 5:
                if (diretorioAtual == NULL)
                {
                    printf("Erro: nenhum diretorio ativo!\n");
                    break;
                }
                if (diretorioAtual->pai != NULL)
                {
                    diretorioAtual = diretorioAtual->pai;
                    printf("Voltou para %s\n", diretorioAtual->nome);
                }
                else
                {
                    printf("Ja esta na root!\n");
                }
                break;

            case 6:
                if (diretorioAtual == NULL)
                {
                    printf("Erro: nenhum diretorio ativo!\n");
                    break;
                }
                { // Ver conteúdo
                    char nome[100];

                    printf("Nome do ficheiro: ");
                    scanf("%s", nome);

                    Ficheiro *f = procurarFicheiro(diretorioAtual->ficheiros, nome);

                    if (f != NULL)
                    {

                        FILE *fp = fopen(f->caminho, "r");

                        if (fp == NULL)
                        {
                            printf("Erro ao abrir ficheiro!\n");
                            continue;
                            ;
                        }

                        char linha[256];

                        printf("\n--- CONTEUDO ---\n");

                        while (fgets(linha, sizeof(linha), fp))
                        {
                            printf("%s", linha);
                        }

                        printf("\n-----------------\n");

                        fclose(fp);
                    }
                    else
                    {
                        printf("Ficheiro nao encontrado!\n");
                    }
                }
                break;

            case 7:
                if (diretorioAtual == NULL)
                {
                    printf("Erro: nenhum diretorio ativo!\n");
                    break;
                }
                {
                    char nome[100];

                    printf("Nome do ficheiro: ");
                    scanf("%s", nome);

                    removerFicheiro(&diretorioAtual->ficheiros, nome);
                    break;
                }

            case 8:
                utilizadorAtual = NULL;
                diretorioAtual = NULL;

                printf("Logout efetuado!\n");
                break;

            case 9:
                printf("Saindo...");
                exit(0);
                break;

            case 10:
                printf("A encerrar sistema...\n");
                exit(0);

            default:
                printf("Opcao invalida!\n");
            }
        }

    } while (1);

    return 0;
}