#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usuario.h"
#include "pasta.h"
#include "ficheiro.h"

int main()
{
    carregarUtilizadores();

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
            printf("3. Remover utilizador\n");
            printf("4. Sair\n");
            printf("======================================\n");
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

                if (utilizadorAtual != NULL)
                {
                    diretorioAtual = procurarSubPasta(utilizadorAtual->home, "Documentos");
                }
            }
            break;

            case 3:
                {
                    char user[30], pass[30];

                    printf("Username: ");
                    scanf("%s", user);

                    printf("Password: ");
                    scanf("%s", pass);

                    removerUtilizador(user, pass);
                }
                break;

            case 4:
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
                diretorioAtual = utilizadorAtual->home;
            }

            printf("\n======================================\n");
            printf(" UTILIZADOR: %s\n", utilizadorAtual->username);
            printf(" DIRETORIO: %s\n", diretorioAtual->nome);
            printf("======================================\n");
            printf("0. Ir para HOME\n");
            printf("1. Listar conteudo\n");
            printf("2. Entrar em pasta\n");
            printf("3. Voltar pasta anterior\n");
            
            printf("\n----------------Gestão de Pastas ----------------------\n\n");
            printf("4. Criar pasta\n");
            printf("5. Eliminar pasta\n");
            
            printf("\n----------------Gestão de Ficheiros -----------------\n\n\n");
            printf("6. Criar ficheiro\n");
            printf("7. Abrir ficheiro\n");
            printf("8. Editar ficheiro\n");
            printf("9. Eliminar ficheiro\n");
            
            printf("\n---------------- Transferências -----------------\n\n\n");
            printf("10. Enviar Ficheiro\n");
            printf("11. Ver ficheiros recebidos\n");

            printf("\n---------------- Comprimir -----------------\n\n\n");
            printf("12. Comprimir ficheiro\n");
            printf("13. Descomprimir ficheiro\n");
            
            printf("\n---------------- Sessão -----------------\n\n\n");
            printf("14. Logout\n");
            printf("15. Sair\n");
            
            printf("======================================\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);

            switch (opcao)
            {

            // ===================================================
            //                    NAVEGAÇÃO
            // ===================================================
            case 0:
                {
                    if (utilizadorAtual == NULL)
                    {
                        printf("Erro: nenhum utilizador logado!\n");
                        break;
                    }

                    if (diretorioAtual == utilizadorAtual->home)
                    {
                        printf("\nJá se encontra na HOME (%s)\n", utilizadorAtual->username);

                        printf("\n[HOME]\n");
                        listarConteudo(utilizadorAtual->home);
                    }
                    else
                    {
                        diretorioAtual = utilizadorAtual->home;

                        printf("\nA voltar para HOME...\n");

                        printf("\n[HOME]\n");
                        listarConteudo(utilizadorAtual->home);
                    }
                }
                break;
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
                    char user[30], pass[30];

                    printf("Username: ");
                    scanf("%s", user);

                    printf("Password: ");
                    scanf("%s", pass);

                    utilizadorAtual = login(user, pass);

                    if (utilizadorAtual != NULL)
                    {
                        diretorioAtual = procurarSubPasta(utilizadorAtual->home, "Documentos");

                        printf("\nLogin efetuado com sucesso!\n");
                        printf("Diretorio atual: %s\n", diretorioAtual->nome);
                    }
                }
                break;

            case 3:
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
            
            // ===================================================
            //                GESTÃO DE PASTAS
            // ===================================================
            case 4:
                 // Criar pasta
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

            case 5:
                // Eliminar pasta
                {
                    if (diretorioAtual == NULL)
                    {
                        printf("Erro: nenhum diretorio ativo!\n");
                        break;
                    }

                    char nome[50];

                    printf("Nome da pasta: ");
                    scanf("%s", nome);

                    removerPasta(diretorioAtual, nome);
                }
                break;

            // ===================================================
            //              GESTÃO DE FICHEIROS
            // ===================================================
            case 6:
                {
                    if (diretorioAtual == NULL)
                    {
                        printf("Erro: nenhum diretório ativo!\n");
                        break;
                    }

                    // Só permitir criação dentro de Documentos
                    if (strcmp(diretorioAtual->nome, "Documentos") != 0)
                    {
                        printf("Erro: só é permitido criar ficheiros dentro de Documentos!\n");
                        break;
                    }

                    char nome[100];
                    char conteudo[256];

                    printf("Nome do ficheiro: ");
                    scanf("%s", nome);

                    // limpar buffer antes do fgets
                    getchar();

                    printf("Conteúdo inicial (pode deixar vazio): ");
                    fgets(conteudo, sizeof(conteudo), stdin);

                    // remover '\n' do fgets
                    conteudo[strcspn(conteudo, "\n")] = '\0';

                    // criar ficheiro físico + lógico
                    Ficheiro *novo = criarFicheiro(nome, diretorioAtual, conteudo);

                    if (novo == NULL)
                    {
                        printf("Erro ao criar ficheiro!\n");
                        break;
                    }

                    // adicionar à lista lógica da pasta
                    adicionarFicheiro(&diretorioAtual->ficheiros, novo);

                    printf("Ficheiro '%s' criado com sucesso em %s!\n",
                        nome,
                        diretorioAtual->caminho);

                    break;
                }

            case 7:
                // Abrir ficheiro
                if (diretorioAtual == NULL)
                {
                    printf("Erro: nenhum diretorio ativo!\n");
                    break;
                }

                {
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
            case 8:
                // Editar ficheiro
                {
                    if (diretorioAtual == NULL)
                    {
                        printf("Erro: nenhum diretório ativo!\n");
                        break;
                    }

                    char nome[100];

                    printf("Nome do ficheiro: ");
                    scanf("%s", nome);

                    Ficheiro *f = procurarFicheiro(diretorioAtual->ficheiros, nome);

                    if (f == NULL)
                    {
                        printf("Ficheiro nao encontrado!\n");
                        break;
                    }

                    editarFicheiro(f);
                }
                break;

            case 9:
                // Eliminar ficheiro
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
                }
                break;

            // ===================================================
            //                TRANSFERÊNCIAS
            // ===================================================
            case 10:
                // Enviar ficheiro  
                printf("Funcionalidade em desenvolvimento...\n");
                break;
            case 11:
                // Receber ficheiro
                printf("Funcionalidade em desenvolvimento...\n");   
                break;

            // ===================================================
            //                  COMPRESSÃO
            // ===================================================
            case 12:
                // Comprimir ficheiro
                printf("Funcionalidade em desenvolvimento...\n");
                break;
            case 13:
                // Descomprimir ficheiro
                printf("Funcionalidade em desenvolvimento...\n");
                break;

            // ===================================================
            //                    SESSÃO
            // ===================================================    
            case 14:
                utilizadorAtual = NULL;
                diretorioAtual = NULL;

                printf("Logout efetuado!\n");
                break;

            case 15:
                printf("Saindo...");
                exit(0);
                break;

            default:
                printf("Opcao invalida!\n");
            }
        }

    } while (1);

    return 0;
}