#include "Memory.h"
#include <stdlib.h>

Memory* createMemory() {
    Memory* memory = malloc(sizeof(Memory));
    memory->dataFile = fopen("dataMem.txt","r+b");
    memory->indexFile = fopen("indexMem.txt","r+b");
    return memory;
}

void writeDataHeader(Memory* memory, DataHeader* header) {
    fseek(memory->dataFile, 0, SEEK_SET);
    fwrite(header, sizeof(DataHeader), 1, memory->dataFile);
}

DataHeader* readDataHeader(Memory* memory) {
    DataHeader* header = malloc(sizeof(DataHeader));
    fseek(memory->dataFile,0, SEEK_SET);
    int success = fread(header, sizeof(DataHeader), 1, memory->dataFile);
    if (!success) {
        header->top = 0;
        header->free = -1;
        writeDataHeader(memory, header);
    }
    return header;
}

Professional* readProfessional(Memory* memory, int pos) {
    Professional* p = malloc(sizeof(Professional));
    fseek(memory->dataFile, sizeof(DataHeader) + pos * sizeof(Professional), SEEK_SET);
    fread(p, sizeof(Professional), 1, memory->dataFile);
    return p;
}

void writeProfessional(Memory* memory, Professional* professional, int pos) {
    fseek(memory->dataFile, sizeof(DataHeader) + pos * sizeof(Professional), SEEK_SET);
    fwrite(professional, sizeof(Professional), 1, memory->dataFile);
}

void printFreePositionsDataHeader(Memory* memory) {
    DataHeader* header = readDataHeader(memory);
    if (header->free == -1) {
        printf("Nenhuma posicao livre no arquivo de dados");
        return;
    }

    int freePos = header->free;
    printf("Posições livres: %d ", freePos);
    free(header);
    while (freePos != -1) {
        Professional* pro = readProfessional(memory,freePos);
        freePos = pro->code;
        printf("%d ", freePos);
        free(pro);
    }
}

void writeIndexHeader(Memory *memory, IndexHeader *header) {
    fseek(memory->indexFile, 0, SEEK_SET);
    fwrite(header, sizeof(IndexHeader), 1, memory->indexFile);
}

IndexHeader *readIndexHeader(Memory *memory) {
    IndexHeader* header = malloc(sizeof(IndexHeader));
    fseek(memory->indexFile,0, SEEK_SET);
    int success = fread(header, sizeof(IndexHeader), 1, memory->indexFile);
    if (!success) {
        header->top = 0;
        header->free = -1;
        header->root = -1;
        writeIndexHeader(memory, header);
    }
    return header;
}

Node *readNode(Memory *memory, int pos) {
    Node* node = malloc(sizeof(Node));
    fseek(memory->indexFile, sizeof(IndexHeader) + pos * sizeof(Node), SEEK_SET);
    fread(node, sizeof(Node), 1, memory->indexFile);
    return node;
}

void writeNode(Memory *memory, Node *node, int pos) {
    fseek(memory->indexFile, sizeof(IndexHeader) + pos * sizeof(Node), SEEK_SET);
    fwrite(node, sizeof(Node), 1, memory->indexFile);
}

void printFreePositionsIndexHeader(Memory *memory) {
    IndexHeader * header = readIndexHeader(memory);
    if (header->free == -1) {
        printf("Nenhuma posicao livre no arquivo de dados");
        return;
    }
    int freePos = header->free;
    printf("Posições livres: %d ", freePos);
    free(header);
    while (freePos != -1) {
        Node* node = readNode(memory,freePos);
        freePos = node->size;
        printf("%d ", freePos);
        free(node);
    }
}