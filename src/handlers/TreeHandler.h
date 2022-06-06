#ifndef BTREE_TREEHANDLER_H
#define BTREE_TREEHANDLER_H
#include "../model/Queue.h"
#include "IndexHandler.h"

typedef struct {
    IndexHandler* indexHandler;
} TreeHandler;

TreeHandler* createTreeHandler(IndexHandler* indexHandler);

void insertKey(TreeHandler* this, int key, int ref);

void removeKey(TreeHandler* this, int key);

int isLeaf(Node* node);

int isOverflowed(Node* node);

int isUnderflowed(Node* node);

int search(TreeHandler* this, int key);

Queue* getReferencesAsc(TreeHandler* this);

#endif