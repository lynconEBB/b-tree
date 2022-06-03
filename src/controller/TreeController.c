#include "TreeController.h"
#include <stdlib.h>

TreeController *createTreeController(Memory* memory) {
    TreeController* treeController = malloc(sizeof(TreeController));
    treeController->memory = memory;
    treeController->node = NULL;
    return treeController;
}


Node* createNode() {
    Node* node = malloc(sizeof(Node));
    node->size = 0;
    for (int i = 0; i < DEGREE; i++) {
        node->children[i] = -1;
    }
    return node;
}

int searchKeyPosition(Node* node, int key, int* pos) {
    for ((*pos) = 0; (*pos) < node->size; (*pos)++) {
        if (key == node->keys[(*pos)])
            return 1;
        else if (key < node->keys[(*pos)])
            break;
    }
    return 0;
}

void addToRight(Node* node, int pos, int key, int ref, int rightChild) {
    for (int i = node->size; i > pos; i--) {
        node->keys[i] = node->keys[i-1];
        node->ref[i] = node->ref[i-1];
        node->children[i+1] = node->children[i];
    }
    node->keys[pos] = key;
    node->ref[pos] = ref;
    node->children[pos+1] = rightChild;
    node->size++;
}

int isOverflowed(Node* node) {
    return node->size == DEGREE;
}

Node* split(Node* overNode, int* medKey, int* medRef) {
    Node* newNode = createNode();
    int q = overNode->size/2;
    newNode->size = overNode->size - q - 1;
    *medKey = overNode->keys[q];
    *medRef = overNode->ref[q];
    newNode->children[0] = overNode->children[q+1];
    for (int i = 0; i < newNode->size; i++) {
        newNode->keys[i] = overNode->keys[q+i+1];
        newNode->ref[i] = overNode->ref[q+i+1];
        newNode->children[i+1] = overNode->children[q+i+2];
    }
    return newNode;
}

Node* insertAux(TreeController* this, int nodePos, int key, int ref) {
     int pos;
     IndexHeader* header = readIndexHeader(this->memory);
     Node* node = readNode(this->memory, nodePos);

     if (!searchKeyPosition(node, key, &pos)) {
        if (isLeaf(node)) {
            addToRight(node, pos, key, ref, -1);
        } else {
            Node* aux = insertAux(this,node->children[pos], key, ref);
            if (isOverflowed(aux)) {
                int medKey, medRef;
                Node* newNode = split(aux,&medKey, &medRef);
                writeNode(this->memory, newNode, header->top);
            }
        }
     }
    return node;
}

void insertKey(TreeController *this, int key, int ref) {
    IndexHeader* header = readIndexHeader(this->memory);
    Node* node;
    if (header->root == -1) {
        header->root = 0;
        node = createNode();
        node->keys[0] = key;
        node->ref[0] = ref;
        node->size++;
        writeNode(this->memory, node, 0);
    } else {
        node = insertAux(this,header->root, key, ref);

    }
}

int isLeaf(Node *node) {
    return node->children[0] == -1;
}
