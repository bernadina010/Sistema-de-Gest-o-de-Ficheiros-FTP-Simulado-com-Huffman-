# Sistema de Gestão de Ficheiros FTP Simulado com Compressão Huffman

## Descrição do Projeto

Este projeto consiste no desenvolvimento de um **Sistema de Gestão de Ficheiros inspirado num servidor FTP simulado**, desenvolvido em linguagem C.

O sistema permite a criação e gestão de utilizadores, organização hierárquica de diretórios, criação e manipulação de ficheiros, transferência de ficheiros entre utilizadores e compressão/descompressão através do **algoritmo de Huffman**.

O projeto foi desenvolvido no âmbito da unidade curricular de **Estruturas de Dados II**, aplicando conceitos como:

- Tipos Abstratos de Dados (TADs);
- Listas ligadas;
- Árvores;
- Heap mínimo;
- Manipulação de ficheiros;
- Alocação dinâmica de memória;
- Algoritmos de compressão sem perdas.


---

# Objetivos

Os principais objetivos do projeto são:

- Desenvolver um sistema de gestão de ficheiros utilizando estruturas de dados dinâmicas;
- Simular o funcionamento básico de um sistema FTP;
- Implementar uma organização hierárquica de pastas;
- Garantir persistência dos dados;
- Permitir comunicação entre utilizadores através da transferência de ficheiros;
- Implementar compressão e descompressão utilizando o algoritmo de Huffman.


---

# Arquitetura do Sistema

O sistema foi organizado através de uma arquitetura modular, dividida nos seguintes TADs:

```
                 MAIN

                  |
  -----------------------------------
  |              |          |        |
Usuario        Pasta    Ficheiro  Huffman

  |              |          |        |
Login       Diretórios  Arquivos  Compressão
Gestão      Árvore      Listas    Descompressão
```

Esta organização permite separar responsabilidades, facilitando a manutenção, testes e evolução do sistema.


---

# Estrutura do Projeto

```
Sistema-de-Gestao-de-Ficheiros/

│
├── main.c
│
├── usuario.c
├── usuario.h
│
├── pasta.c
├── pasta.h
│
├── ficheiro.c
├── ficheiro.h
│
├── huffman.c
├── huffman.h
│
├── utilizadores.txt
│
└── README.md
```

---

#  Gestão de Utilizadores

O módulo de utilizadores permite:

- Criar novos utilizadores;
- Efetuar autenticação através de login;
- Terminar sessão;
- Remover utilizadores;
- Guardar e carregar utilizadores através de ficheiros de persistência.


Cada utilizador possui uma diretoria principal (`home`) onde são armazenados os seus ficheiros e pastas.

Estrutura inicial:

```
Utilizador

└── Home
    |
    ├── Documentos
    |
    └── Recebidos
```

---

#  Gestão de Diretórios

A gestão de pastas é baseada numa estrutura de **árvore hierárquica**, semelhante à utilizada pelos sistemas operativos.

Cada pasta possui:

- Nome;
- Caminho físico;
- Referência para a pasta pai;
- Primeiro filho;
- Próximo irmão;
- Lista de ficheiros.


Funcionalidades implementadas:

- Criar pastas;
- Criar subpastas;
- Navegar entre diretórios;
- Listar conteúdo;
- Remover pastas.


Exemplo:

```
Home

├── Documentos
│      |
│      └── trabalho.txt
│
└── Recebidos
       |
       └── ficheiro_recebido.txt
```

---

#  Gestão de Ficheiros

O módulo de ficheiros permite:

- Criar ficheiros;
- Guardar conteúdo inicial;
- Editar ficheiros;
- Ler ficheiros;
- Listar ficheiros;
- Controlar o armazenamento físico.


Cada ficheiro contém:

- Nome;
- Tamanho;
- Caminho físico;
- Referência para o próximo ficheiro.


---

#  Transferência de Ficheiros

O sistema permite a transferência de ficheiros entre utilizadores.

O processo realizado é:

1. O utilizador seleciona um ficheiro;
2. Indica o utilizador destinatário;
3. O sistema copia o ficheiro físico;
4. O ficheiro é registado na pasta `Recebidos` do destinatário.


Exemplo:

```
Utilizador A

Documentos
    |
    └── relatorio.txt


            Transferência


Utilizador B

Recebidos
    |
    └── relatorio.txt
```

---

#  Compressão Huffman

O módulo `huffman.c` e `huffman.h` implementa um algoritmo de compressão **sem perdas baseado na codificação de Huffman**.

O processo de compressão é composto pelas seguintes etapas:

## 1. Contagem de Frequências

O sistema analisa o ficheiro original e calcula a frequência de ocorrência de cada caractere.


## 2. Construção do Heap Mínimo

As frequências obtidas são utilizadas para criar um Heap Mínimo, permitindo selecionar sempre os elementos com menor frequência.


## 3. Construção da Árvore de Huffman

A partir do Heap é construída a árvore binária de Huffman, onde cada caminho representa um código binário.


## 4. Geração dos Códigos

Cada caractere recebe um código binário único baseado no percurso realizado na árvore.


## 5. Criação do ficheiro comprimido

O conteúdo original é convertido para a sequência de bits correspondente e armazenado num novo ficheiro:

```
exemplo.txt

      ↓

exemplo.txt.huff
```

---

#  Descompressão Huffman

A descompressão permite recuperar o conteúdo original através de:

1. Leitura do cabeçalho do ficheiro comprimido;
2. Reconstrução da árvore de Huffman;
3. Leitura dos bits armazenados;
4. Percurso da árvore;
5. Recuperação dos caracteres originais.


O sistema garante que o ficheiro recuperado possui o mesmo conteúdo do ficheiro inicial.


---

#  Estruturas de Dados Utilizadas

## Lista Ligada

Utilizada para:

- Armazenamento de utilizadores;
- Lista de ficheiros dentro das pastas.


## Árvore

Utilizada para:

- Representação dos diretórios;
- Construção da árvore de Huffman.


## Heap Mínimo

Utilizado no algoritmo de Huffman para organizar os nós pela menor frequência.


---

#  Tecnologias Utilizadas

- Linguagem C;
- Estruturas de Dados;
- Ponteiros;
- Gestão dinâmica de memória;
- Manipulação de ficheiros;
- Git/GitHub.


---

#  Compilação

Para compilar o projeto:

```bash
gcc main.c usuario.c pasta.c ficheiro.c huffman.c -o sistema
```

Executar:

```bash
./sistema
```


---

#  Testes Realizados

Foram realizados testes para validar:

✔ Criação de utilizadores;  
✔ Login e logout;  
✔ Persistência dos dados;  
✔ Criação de pastas;  
✔ Criação e edição de ficheiros;  
✔ Transferência entre utilizadores;  
✔ Visualização de ficheiros recebidos;  
✔ Compressão Huffman;  
✔ Descompressão e recuperação dos ficheiros originais.


---

#  Bibliografia

- CORMEN, Thomas H.; LEISERSON, Charles E.; RIVEST, Ronald L.; STEIN, Clifford.  
  *Introduction to Algorithms*. 4th Edition. MIT Press, 2022.

- SALOMON, David; MOTTA, Giovanni.  
  *Handbook of Data Compression*. 5th Edition. Springer, 2010.

- WEISS, Mark Allen.  
  *Data Structures and Algorithm Analysis in C*. 2nd Edition. Addison-Wesley, 1997.

- BASS, Len; CLEMENTS, Paul; KAZMAN, Rick.  
  *Software Architecture in Practice*. 4th Edition. Addison-Wesley Professional, 2021.


---

#  Conclusão

O desenvolvimento deste projeto permitiu aplicar conceitos fundamentais de programação estruturada e estruturas de dados na criação de um sistema completo de gestão de ficheiros.

A implementação das estruturas de árvores, listas ligadas e Heap Mínimo possibilitou representar de forma eficiente a organização dos dados, enquanto o algoritmo de Huffman demonstrou uma aplicação prática de compressão de dados sem perdas.

O resultado final é um sistema modular, funcional e preparado para futuras expansões.
