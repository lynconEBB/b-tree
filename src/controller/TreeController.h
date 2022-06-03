#ifndef BTREE_TREECONTROLLER_H
#define BTREE_TREECONTROLLER_H
#include "../memory/Memory.h"
#include "../model/BTree.h"

typedef struct {
    Memory* memory;
    Node* node;
} TreeController;

TreeController* createTreeController(Memory* memory);

void insertKey(TreeController* this, int key, int ref);

int isLeaf(Node* node);

#endif