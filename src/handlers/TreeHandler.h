#ifndef BTREE_TREECONTROLLER_H
#define BTREE_TREECONTROLLER_H
#include "../memory/Memory.h"
#include "../model/BTree.h"
#include "../model/Queue.h"

typedef struct {
    Memory* memory;
} TreeHandler;

TreeHandler* createTreeController(Memory* memory);

void insertKey(TreeHandler* this, int key, int ref);

void removeKey(TreeHandler* this, int key);

int isLeaf(Node* node);

int isOverflowed(Node* node);

int isUnderflowed(Node* node);

int search(TreeHandler* this, int key);

Queue* getReferencesAsc(TreeHandler* this);

#endif