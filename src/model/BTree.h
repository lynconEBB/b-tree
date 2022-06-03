#ifndef BTREE_BTREE_H
#define BTREE_BTREE_H

#define DEGREE 5

typedef struct{
    int ref[DEGREE];
    int keys[DEGREE];
    int children[DEGREE+1];
    int size;
} Node;

#endif
