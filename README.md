# Sistema de Gestão de Ficheiros (FTP Simulado com Huffman)

Projeto desenvolvido para a disciplina de **Estruturas de Dados II**, implementado na linguagem **C**, simulando um sistema de gestão de ficheiros inspirado no funcionamento de um servidor FTP.

O sistema permite a gestão de utilizadores, diretórios e ficheiros, recorrendo a estruturas de dados dinâmicas e manipulação de ficheiros, culminando com a implementação de compressão através do algoritmo de Huffman.

> ⚠️ Estado atual: Projeto em desenvolvimento.

---

# Objetivos

- Aplicar Estruturas de Dados na resolução de um problema real.
- Desenvolver um sistema modular utilizando TADs.
- Trabalhar com ficheiros reais.
- Implementar persistência de dados.
- Simular a transferência de ficheiros entre utilizadores.
- Implementar compressão e descompressão utilizando Huffman.

---

# 🛠 Tecnologias

- Linguagem C
- GCC
- Git
- GitHub
- GitHub Codespaces

---

# 📂 Estrutura atual do projeto

```
SistemaGestaoFicheiros/

├── main.c
├── usuario.c
├── usuario.h
├── pasta.c
├── pasta.h
├── ficheiro.c
├── ficheiro.h
├── utilizadores.txt
└── README.md
```

---

# 🧱 Estruturas de Dados utilizadas

## Lista Ligada

Utilizada para:

- Utilizadores
- Ficheiros
- Subpastas

---

## Árvore Geral

Utilizada para representar a estrutura lógica de diretórios.

```
root
├── Documentos
│   ├── Faculdade
│   └── Fotos
└── Recebidos
```

Cada pasta pode possuir:

- várias subpastas;
- vários ficheiros;
- referência para a pasta pai.

---

# ✅ Funcionalidades implementadas

## Gestão de Utilizadores

- [x] Criar utilizador
- [x] Login
- [x] Logout
- [x] Persistência em ficheiro (`utilizadores.txt`)
- [x] Carregamento automático dos utilizadores
- [x] Impedir utilizadores duplicados
- [x] Remover utilizador
- [x] Remover pasta física do utilizador

---

## Gestão de Pastas

- [x] Criação da árvore lógica
- [x] Pasta Root
- [x] Pasta Documentos
- [x] Pasta Recebidos
- [x] Criar pasta
- [x] Navegação entre pastas
- [x] Listar conteúdo

---

# 🚧 Funcionalidades em desenvolvimento

## Gestão de Ficheiros

- [ ] Criar ficheiro
- [ ] Abrir ficheiro
- [ ] Editar ficheiro
- [ ] Remover ficheiro

---

## Transferência de Ficheiros

- [ ] Enviar ficheiro
- [ ] Receber ficheiro
- [ ] Visualizar ficheiros recebidos

---

## Compressão

- [ ] Compressão Huffman
- [ ] Descompressão Huffman

---

# 📋 Menu principal

## Sem utilizador autenticado

```
1. Criar utilizador
2. Login
3. Remover utilizador
4. Sair
```

---

## Com utilizador autenticado

```
1. Listar conteúdo
2. Criar ficheiro
3. Criar pasta
4. Entrar em pasta
5. Remover pasta
6. Voltar pasta anterior
7. Eliminar ficheiro
8. Editar ficheiro
9. Eliminar pasta
10. Enviar Ficheiro
11. Ver ficheiros recebidos
12. Comprimir ficheiro
13. Descomprimir ficheiro
14. Logout
15. Sair
```

---


# Estado do projeto

🚧 Em desenvolvimento.
