#ifndef BTREE_INDEXHANDLER_H
#define BTREE_INDEXHANDLER_H
#include <stdio.h>

#define DEGREE 5

typedef struct{
    int ref[DEGREE];
    int keys[DEGREE];
    int children[DEGREE+1];
    int size;
} Node;

typedef struct {
    int top;
    int free;
    int root;
} IndexHeader;

typedef struct {
    FILE* file;
    IndexHeader* header;
} IndexHandler;

IndexHandler* createIndexHandler();

void writeIndexHeader(IndexHandler* indexHandler);

IndexHeader* readIndexHeader(IndexHandler* indexHandler);

IndexHeader* getIndexHeader(IndexHandler* this);

void printFreePositionsIndexHeader(IndexHandler* indexHandler);

Node* readNode(IndexHandler* indexHandler, int pos);

void writeNode(IndexHandler* indexHandler, Node* node, int pos);

int addNode(IndexHandler* indexHandler, Node* node);

void removeNode(IndexHandler* indexHandler, int pos);

#endif