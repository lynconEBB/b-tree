#ifndef BTREE_MEMORY_H
#define BTREE_MEMORY_H
#include <stdio.h>
#include "../model/BTree.h"
#include "../model/Professional.h"

typedef struct {
    int top;
    int free;
} DataHeader;

typedef struct {
    int top;
    int free;
    int root;
} IndexHeader;

typedef struct {
    FILE* indexFile;
    FILE* dataFile;
} Memory;

Memory* createMemory();

void writeDataHeader(Memory* memory, DataHeader* header);

DataHeader* readDataHeader(Memory* memory);

void printFreePositionsDataHeader(Memory* memory);

Professional* readProfessional(Memory* memory, int pos);

void writeProfessional(Memory* file, Professional* professional, int pos);

void writeIndexHeader(Memory* memory, IndexHeader* header);

IndexHeader* readIndexHeader(Memory* memory);

void printFreePositionsIndexHeader(Memory* memory);

Node* readNode(Memory* memory, int pos);

void writeNode(Memory* memory, Node* node, int pos);

#endif