#ifndef BTREE_TREEHANDLER_H
#define BTREE_TREEHANDLER_H
#include "../model/Queue.h"
#include "IndexHandler.h"

// Estrutura do manipulador de Arvore B
// Utiliza o manipulaodr do arquivo de indices para salvar as modificações
typedef struct {
    IndexHandler* indexHandler;
} TreeHandler;

// Cria um nó vazio
// Pré-condição: Nenhuma
// Pós-condição: Ponteiro para nó sem filhos e com tamanho 0;
Node *createNode();

// Cria um manipulador de arvore B recebendo um manipulador de indices
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Manipulador de aruvore B retornado
TreeHandler* createTreeHandler(IndexHandler* indexHandler);

// Insere uma chave e uma referencia na arvore B realizando as devidas
// correçoes caso o nó fiquem em overflow
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Chave e referencia adicionadas
void insertKey(TreeHandler* this, int key, int ref);

// Remove uma chave da arvore B realizando as devidas correçoes
// caso o nó fique em underflow
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Chave removida da arvore
void removeKey(TreeHandler* this, int key);

// Veritica se o nó fornecido é folha
// Pré-condição: Ponteiro para manipulador de indices válido
// Pós-condição: Nenhuma
int isLeaf(Node* node);

// Verifica se o nó fornecido esta em overflow
// Pré-condição: Ponteiro para nó válido
// Pós-condição: Nenhuma
int isOverflowed(Node* node);

// Verifica se o nó fornecido esta em underflow
// Pré-condição: Ponteiro para nó válido
// Pós-condição: Nenhuma
int isUnderflowed(Node* node);

// Procura a referencia da chave fornecida na arvore B
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Retorna a referencia da chave, caso não seja encontrada,
// -1 é retornado
int search(TreeHandler* this, int key);

// Fornece as referencias em ordem crescente
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Retorna uma fila contendo as referencias
Queue* getReferencesAsc(TreeHandler* this);

#endif