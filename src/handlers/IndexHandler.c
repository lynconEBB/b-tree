#include "IndexHandler.h"
#include <stdlib.h>

// Cria um manipulador do arquivo de indices, iniciando o cabeçalho e criando um arquivo
// caso não exista
// Pré-condição: Nenhuma
// Pós-condição: Ponteiro válido para um manipulador de indices
IndexHandler *createIndexHandler() {
    IndexHandler* this = malloc(sizeof(IndexHandler));
    char fileName[] = "indexFile.bin";
    this->file = fopen(fileName,"r+b");
    if (this->file == NULL)  {
        this->file = fopen(fileName, "w");
        fclose(this->file);
        this->file = fopen(fileName,"r+b");
    }
    this->header = readIndexHeader(this);
    return this;
}

// Liber memoria utilizada pelo manipulador de indices e fecha arquivo utilizado
// Pré-condição: Ponteiro para manipulador de indices valido e ponteiro para profissional
// Pós-condição: Manipulador de indices invalidado
void freeIndexHandler(IndexHandler *this) {
    fclose(this->file);
    free(this->header);
    free(this);
}

// Adiciona novo nó ao arquivo de indices, caso nenhuma posiçao livre exista
// o nó é adicionado no topo do arquivo, caso contrario a posicao livre é utilizada
// Pré-condição: Ponteiro para manipulador de indices valido e ponteiro para nó
// Pós-condição: Retorna a posição do nó no arquivo de indices
void removeNode(IndexHandler* this, int filePos) {
    Node node;
    node.size = this->header->free;
    this->header->free = filePos;
    writeIndexHeader(this);
    writeNode(this, &node, filePos);
}

// Adiciona novo nó ao arquivo de indices, caso nenhuma posiçao livre exista
// o nó é adicionado no topo do arquivo, caso contrario a posicao livre é utilizada
// Pré-condição: Ponteiro para manipulador de indices valido e ponteiro para nó
// Pós-condição: Retorna a posição do nó no arquivo de indices
int addNode(IndexHandler* this, Node *node) {
    int pos;
    if (this->header->free == -1) {
        pos = this->header->top;
        this->header->top++;
    } else {
        pos = this->header->free;
        Node *aux = readNode(this, this->header->free);
        this->header->free = aux->size;
        free(aux);
    }
    writeNode(this, node, pos);
    writeIndexHeader(this);
    return pos;
}

// Fornece o cabeçalho de indices atual
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Ponteiro para cabeçalho atual
IndexHeader* getIndexHeader(IndexHandler* this) {
    return this->header;
}

// Lê e retorna o cabeçalho do arquivo de indices mo
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Caso o cabeçalho não seja encontrado no arquivo um cabeçalho novo é adicionado,
// o ponteiro de cabeçalho do manipulador de indices é preenchido com o cabeçalho lido
IndexHeader* readIndexHeader(IndexHandler* this) {
    free(this->header);
    this->header = malloc(sizeof(IndexHeader));
    fseek(this->file,0, SEEK_SET);
    int success = fread(this->header, sizeof(IndexHeader), 1, this->file);
    if (!success) {
        this->header->top = 0;
        this->header->free = -1;
        this->header->root = -1;
        writeIndexHeader(this);
    }
    return this->header;
}

// Escreve o cabeçalho atual no arquivo de indices
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Cabeçalho salvo no arquivo
void writeIndexHeader(IndexHandler* this) {
    fseek(this->file, 0, SEEK_SET);
    fwrite(this->header, sizeof(IndexHeader), 1, this->file);
}

// Lê o nó do arquivo de indices na posição fornecida
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Retorna nó lido na posição
Node* readNode(IndexHandler* this, int pos) {
    Node* node = malloc(sizeof(Node));
    fseek(this->file, sizeof(IndexHeader) + (pos * sizeof(Node)), SEEK_SET);
    fread(node, sizeof(Node), 1, this->file);
    return node;
}

// Escreve o nó fornecido no arquivo de indices na posição fornecida
// Pré-condição: Ponteiro para manipulador de indices, nó valida
// Pós-condição: Profissional salvo no arquivo de indices
void writeNode(IndexHandler* this, Node *node, int pos) {
    fseek(this->file, sizeof(IndexHeader) + (pos * sizeof(Node)), SEEK_SET);
    fwrite(node, sizeof(Node), 1, this->file);
}

// Imprime as posições livres do arquivo
// Pré-condição: Ponteiro para manipulador de indices valido
// Pós-condição: Nenhuma
void printFreePositionsIndexHeader(IndexHandler* this) {
    if (this->header->free == -1) {
        printf("Nenhuma posicao livre no arquivo de indices");
        return;
    }
    int freePos = this->header->free;
    printf("Posições livres: %d ", freePos);
    while (freePos != -1) {
        Node* node = readNode(this,freePos);
        freePos = node->size;
        if (freePos != -1)
            printf("%d ", freePos);
        free(node);
    }
}

