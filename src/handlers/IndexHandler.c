#include "IndexHandler.h"
#include <stdlib.h>

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

void removeNode(IndexHandler* this, int filePos) {
    Node node;
    node.size = this->header->free;
    this->header->free = filePos;
    writeIndexHeader(this);
    writeNode(this, &node, filePos);
}

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

IndexHeader* getIndexHeader(IndexHandler* this) {
    return this->header;
}

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
void writeIndexHeader(IndexHandler* this) {
    fseek(this->file, 0, SEEK_SET);
    fwrite(this->header, sizeof(IndexHandler), 1, this->file);
}

Node* readNode(IndexHandler* this, int pos) {
    Node* node = malloc(sizeof(Node));
    fseek(this->file, sizeof(IndexHeader) + pos * sizeof(Node), SEEK_SET);
    fread(node, sizeof(Node), 1, this->file);
    return node;
}

void writeNode(IndexHandler* this, Node *node, int pos) {
    fseek(this->file, sizeof(IndexHeader) + pos * sizeof(Node), SEEK_SET);
    fwrite(node, sizeof(Node), 1, this->file);
}

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
        printf("%d ", freePos);
        free(node);
    }
}

