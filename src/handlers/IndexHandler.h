#ifndef BTREE_INDEXHANDLER_H
#define BTREE_INDEXHANDLER_H
#include <stdio.h>

// Ordem da arvore B
#define DEGREE 5

// Nó da arvore B
typedef struct{
    int ref[DEGREE];
    int keys[DEGREE];
    int children[DEGREE+1];
    int size;
} Node;

// Estrutura do cabeçalho do arquivo de indices
typedef struct {
    int top;
    int free;
    int root;
} IndexHeader;

// Estrutura do Manipulador de indices
// Utilizado para gerenciamento das informações no arquivo de indices
typedef struct {
    FILE* file;
    IndexHeader* header;
} IndexHandler;

// Cria um manipulador do arquivo de indices, iniciando o cabeçalho e criando um arquivo
// caso não exista
// Pré-condição: Nenhuma
// Pós-condição: Ponteiro válido para um manipulador de indices
IndexHandler* createIndexHandler();

// Escreve o cabeçalho atual no arquivo de indices
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Cabeçalho salvo no arquivo
void writeIndexHeader(IndexHandler* indexHandler);

// Lê e retorna o cabeçalho do arquivo de indices mo
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Caso o cabeçalho não seja encontrado no arquivo um cabeçalho novo é adicionado,
// o ponteiro de cabeçalho do manipulador de indices é preenchido com o cabeçalho lido
IndexHeader* readIndexHeader(IndexHandler* indexHandler);

// Fornece o cabeçalho de indices atual
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Ponteiro para cabeçalho atual
IndexHeader* getIndexHeader(IndexHandler* this);

// Imprime as posições livres do arquivo
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Nenhuma
void printFreePositionsIndexHeader(IndexHandler* indexHandler);

// Lê o nó do arquivo de indices na posição fornecida
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Retorna nó lido na posição
Node* readNode(IndexHandler* indexHandler, int pos);

// Escreve o nó fornecido no arquivo de indices na posição fornecida
// Pré-condição: Ponteiro para manipulador de indices, nó valido
// Pós-condição: Profissional salvo no arquivo de indices
void writeNode(IndexHandler* indexHandler, Node* node, int pos);

// Adiciona novo nó ao arquivo de indices, caso nenhuma posiçao livre exista
// o nó é adicionado no topo do arquivo, caso contrario a posicao livre é utilizada
// Pré-condição: Ponteiro para manipulador de indices valido e ponteiro para nó
// Pós-condição: Retorna a posição do nó no arquivo de indices
int addNode(IndexHandler* indexHandler, Node* node);

// Adiciona novo nó ao arquivo de indices, caso nenhuma posiçao livre exista
// o nó é adicionado no topo do arquivo, caso contrario a posicao livre é utilizada
// Pré-condição: Ponteiro para manipulador de indices valido e ponteiro para nó
// Pós-condição: Retorna a posição do nó no arquivo de indices
void removeNode(IndexHandler* indexHandler, int pos);

#endif