#ifndef BTREE_TREECONTROLLER_H
#define BTREE_TREECONTROLLER_H
#include "../memory/Memory.h"

typedef struct {
    Memory* memory;
} TreeController;

TreeController* createTreeController(Memory* memory);

void insertKey(TreeController* this, int key, int ref);

#endif