#include "TreeController.h"
#include <stdlib.h>
#include "../model/Queue.h"

TreeController *createTreeController(Memory* memory) {
    TreeController* treeController = malloc(sizeof(TreeController));
    treeController->memory = memory;
    treeController->node = NULL;
    return treeController;
}

Node* createNode() {
    Node* node = malloc(sizeof(Node));
    node->size = 0;
    for (int i = 0; i < DEGREE + 1; i++) {
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

void aux_printAsc(TreeController* this, int nodePos)
{
    if (nodePos != -1 ) {
        Node* node = readNode(this->memory, nodePos);
        for(int i = 0; i < node->size; i++ ) {
            aux_printAsc(this, node->children[i]);
            printf("%d ", node->keys[i]);
        }
        aux_printAsc(this, node->children[node->size]);
        free(node);
    }
}

void printAsc(TreeController* this)
{
    IndexHeader* header = readIndexHeader(this->memory);
    aux_printAsc(this, header->root);
    printf("\n");
    free(header);
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

int isOverflowed(Node* node)
{
    return node->size == DEGREE;
}

Node* split(Node* overNode, int* medKey, int* medRef) {
    Node* newNode = createNode();
    int q = overNode->size/2;
    newNode->size = overNode->size - q - 1;
    *medKey = overNode->keys[q];
    *medRef = overNode->ref[q];
    overNode->size = q;
    newNode->children[0] = overNode->children[q+1];
    for (int i = 0; i < newNode->size; i++) {
        newNode->keys[i] = overNode->keys[q+i+1];
        newNode->ref[i] = overNode->ref[q+i+1];
        newNode->children[i+1] = overNode->children[q+i+2];
    }
    return newNode;
}

int saveNewNode(TreeController* this, Node* node) {
    IndexHeader* header = readIndexHeader(this->memory);
    int pos;
    if (header->free == -1) {
        pos = header->top;
        header->top++;
    } else {
        pos = header->free;
        Node* aux = readNode(this->memory,header->free);
        header->free = aux->size;
        free(aux);
    }
    writeNode(this->memory,node,pos);
    writeIndexHeader(this->memory, header);
    free(header);
    return pos;
}

Node* insertAux(TreeController* this, int nodePos, int key, int ref) {
     int pos;
     Node* node = readNode(this->memory, nodePos);

     if (!searchKeyPosition(node, key, &pos)) {
        if (isLeaf(node)) {
            addToRight(node, pos, key, ref, -1);
        } else {
            Node* aux = insertAux(this,node->children[pos], key, ref);
            if (isOverflowed(aux)) {
                int medKey, medRef;
                Node* newNode = split(aux,&medKey, &medRef);
                int newRef = saveNewNode(this, newNode);
                addToRight(node, pos, medKey, medRef, newRef);
                saveNewNode(this, aux);
                free(newNode);
            }
            writeNode(this->memory, aux, node->children[pos]);
            free(aux);
        }
     }
    return node;
}

void insertKey(TreeController *this, int key, int ref) {
    IndexHeader* header = readIndexHeader(this->memory);
    Node* node;
    if (header->root == -1) {
        header->root = 0;
        header->top++;
        writeIndexHeader(this->memory, header);
        node = createNode();
        node->keys[0] = key;
        node->ref[0] = ref;
        node->size++;
        writeNode(this->memory, node, 0);
    } else {
        node = insertAux(this,header->root, key, ref);
        if (isOverflowed(node)) {
            int medKey, medRef;
            Node* aux = split(node, &medKey, &medRef);
            int auxRef = saveNewNode(this, aux);
            Node* newRoot = createNode();
            newRoot->keys[0] = medKey;
            newRoot->ref[0] = medRef;
            newRoot->children[0] = header->root;
            newRoot->children[1] = auxRef;
            for (int i = (DEGREE/2)+1; i < DEGREE; i++)
                node->children[i] = -1;
            newRoot->size = 1;
            writeNode(this->memory, node, header->root);
            int rootPos = saveNewNode(this, newRoot);
            header = readIndexHeader(this->memory);
            header->root = rootPos;
            writeIndexHeader(this->memory, header);
        } else {
            writeNode(this->memory, node, header->root);
        }
    }
    free(node);
    free(header);
}

void printNode(Node* node) {
    printf("[ ");
    for (int i = 0; i < node->size; i++ ) {
        printf("%d ", node->keys[i]);
    }
    printf("] ");
}

void printByLevel(TreeController* this) {
    IndexHeader* header = readIndexHeader(this->memory);
    if (header->root == -1)
        return;

    Queue* queue = newQueue();
    enqueue(queue,header->root);
    enqueue(queue, -1);
    while (1) {
        int pos = queue->first->data;
        dequeue(queue);
        if (pos != -1) {
            Node* node = readNode(this->memory, pos);
            printNode(node);
            for (int i = 0; node->children[i] != -1; i++) {
                enqueue(queue, node->children[i]);
            }
            free(node);
        } else {
            printf("\n");
            if (isEmpty(queue))
                break;
            enqueue(queue, -1);
        }
    }
}

int isLeaf(Node *node)
{
    return node->children[0] == -1;
}