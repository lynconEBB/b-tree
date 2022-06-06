#include "TreeHandler.h"
#include <stdlib.h>

TreeHandler *createTreeHandler(IndexHandler* indexHandler) {
    TreeHandler *treeController = malloc(sizeof(TreeHandler));
    treeController->indexHandler = indexHandler;
    return treeController;
}

Node *createNode() {
    Node *node = malloc(sizeof(Node));
    node->size = 0;
    for (int i = 0; i < DEGREE + 1; i++) {
        node->children[i] = -1;
    }
    return node;
}

int isLeaf(Node *node) {
    return node->children[0] == -1;
}

int isOverflowed(Node *node) {
    return node->size == DEGREE;
}

int isUnderflowed(Node *node) {
    return node->size < DEGREE / 2;
}

void aux_getReferencesAsc(TreeHandler *this, Queue *queue, int nodePos) {
    if (nodePos != -1) {
        Node *node = readNode(this->indexHandler, nodePos);
        for (int i = 0; i < node->size; i++) {
            aux_getReferencesAsc(this, queue, node->children[i]);
            enqueue(queue, node->ref[i]);
        }
        aux_getReferencesAsc(this, queue, node->children[node->size]);
        free(node);
    }
}

Queue* getReferencesAsc(TreeHandler *this) {
    Queue *ascRefs = newQueue();
    IndexHeader *header = getIndexHeader(this->indexHandler);
    aux_getReferencesAsc(this, ascRefs, header->root);
    return ascRefs;
}

int aux_search(TreeHandler *this, int filePos, int key, int *pos) {
    if (filePos == -1)
        return -1;
    Node *node = readNode(this->indexHandler, filePos);

    int i = 0;
    while (i < node->size && node->keys[i] < key) {
        i++;
    }
    if (i + 1 > node->size || node->keys[i] > key)
        return aux_search(this, node->children[i], key, pos);
    *pos = i;
    return filePos;
}

int search(TreeHandler *this, int key) {
    int pos, ref;
    IndexHeader *header = getIndexHeader(this->indexHandler);

    if (header->root == -1)
        ref = -1;
    else
        ref = aux_search(this, header->root, key, &pos);

    if (ref != -1) {
        Node *node = readNode(this->indexHandler, ref);
        ref = node->ref[pos];
        free(node);
    }

    return ref;
}

/**
 * Busca a posicao em que a chave esta ou estaria no nó
 * Retorna 1 se a chave esta presente e 0 caso contrario
 */
int searchKeyPosition(Node *node, int key, int *pos) {
    for ((*pos) = 0; (*pos) < node->size; (*pos)++) {
        if (key == node->keys[(*pos)])
            return 1;
        else if (key < node->keys[(*pos)])
            break;
    }
    return 0;
}


void addToRight(Node *node, int pos, int key, int ref, int rightChild) {
    for (int i = node->size; i > pos; i--) {
        node->keys[i] = node->keys[i - 1];
        node->ref[i] = node->ref[i - 1];
        node->children[i + 1] = node->children[i];
    }
    node->keys[pos] = key;
    node->ref[pos] = ref;
    node->children[pos + 1] = rightChild;
    node->size++;
}

void addToLeft(Node *node, int pos, int key, int ref, int leftChild) {
    for (int i = node->size; i > pos; i--) {
        node->keys[i] = node->keys[i - 1];
        node->ref[i] = node->ref[i - 1];
        node->children[i + 1] = node->children[i];
    }
    node->children[pos+1] = node->children[pos];
    node->keys[pos] = key;
    node->ref[pos] = ref;
    node->children[pos] = leftChild;
    node->size++;
}


Node *split(Node *overNode, int *medKey, int *medRef) {
    Node *newNode = createNode();
    int q = overNode->size / 2;
    newNode->size = overNode->size - q - 1;
    *medKey = overNode->keys[q];
    *medRef = overNode->ref[q];
    overNode->size = q;
    newNode->children[0] = overNode->children[q + 1];
    for (int i = 0; i < newNode->size; i++) {
        newNode->keys[i] = overNode->keys[q + i + 1];
        newNode->ref[i] = overNode->ref[q + i + 1];
        newNode->children[i + 1] = overNode->children[q + i + 2];
    }
    return newNode;
}


Node *insertAux(TreeHandler *this, int nodePos, int key, int ref) {
    int pos;
    Node *node = readNode(this->indexHandler, nodePos);

    if (!searchKeyPosition(node, key, &pos)) {
        if (isLeaf(node)) {
            addToRight(node, pos, key, ref, -1);
        } else {
            Node *aux = insertAux(this, node->children[pos], key, ref);
            if (isOverflowed(aux)) {
                int medKey, medRef;
                Node *newNode = split(aux, &medKey, &medRef);
                int newRef = addNode(this->indexHandler, newNode);
                addToRight(node, pos, medKey, medRef, newRef);
                addNode(this->indexHandler, aux);
                free(newNode);
            }
            writeNode(this->indexHandler, aux, node->children[pos]);
            free(aux);
        }
    }
    return node;
}

void insertKey(TreeHandler *this, int key, int ref) {
    IndexHeader *header = getIndexHeader(this->indexHandler);
    Node *node;
    if (header->root == -1) {
        header->root = 0;
        header->top++;
        writeIndexHeader(this->indexHandler);
        node = createNode();
        node->keys[0] = key;
        node->ref[0] = ref;
        node->size++;
        writeNode(this->indexHandler, node, 0);
    } else {
        node = insertAux(this, header->root, key, ref);
        if (isOverflowed(node)) {
            int medKey, medRef;
            Node *aux = split(node, &medKey, &medRef);
            int auxRef = addNode(this->indexHandler, aux);
            Node *newRoot = createNode();
            newRoot->keys[0] = medKey;
            newRoot->ref[0] = medRef;
            newRoot->children[0] = header->root;
            newRoot->children[1] = auxRef;
            for (int i = (DEGREE / 2) + 1; i < DEGREE; i++)
                node->children[i] = -1;
            newRoot->size = 1;
            writeNode(this->indexHandler, node, header->root);
            int rootPos = addNode(this->indexHandler, newRoot);
            header->root = rootPos;
            writeIndexHeader(this->indexHandler);
        } else {
            writeNode(this->indexHandler, node, header->root);
        }
    }
    free(node);
}

void removeFromNode(Node *node, int pos) {
    for (int i = pos; i < node->size; i++) {
        node->keys[i] = node->keys[i + 1];
        node->ref[i] = node->ref[i + 1];
        node->children[i + 1] = node->children[i + 2];
    }
    node->size--;
}

void getMaxKeyAndRef(TreeHandler *this, int nodePos, int *key, int *ref) {
    Node *node = readNode(this->indexHandler, nodePos);
    int max;
    if (isLeaf(node)) {
        *key = node->keys[node->size - 1];
        *ref = node->ref[node->size - 1];
        free(node);
    } else {
        int nextPos = node->children[node->size];
        free(node);
        getMaxKeyAndRef(this, nextPos, key, ref);
    }
}

int borrowLeft(TreeHandler *this, Node *parent, Node* underFlowed, int underPos) {
    if (underPos - 1 < 0)
        return 0;

    Node *leftChild = readNode(this->indexHandler, parent->children[underPos - 1]);
    if (leftChild->size <= DEGREE/2){
        free(leftChild);
        return 0;
    }

    addToRight(underFlowed,0,parent->keys[underPos-1],parent->keys[underPos-1],leftChild->children[leftChild->size]);
    leftChild->size--;
    parent->keys[underPos - 1] = leftChild->keys[leftChild->size];
    writeNode(this->indexHandler,leftChild,parent->children[underPos - 1]);

    free(leftChild);
    return 1;
}

int borrowRight(TreeHandler *this, Node *parent, Node* underFlowed, int underPos) {
    Node *rightChild = readNode(this->indexHandler, parent->children[underPos + 1]);
    if (rightChild->size <= DEGREE / 2){
        free(rightChild);
        return 0;
    }

    underFlowed->keys[underFlowed->size] = parent->keys[underPos];
    underFlowed->size++;
    underFlowed->children[underFlowed->size] = rightChild->children[0];

    parent->keys[underPos] = rightChild->keys[0];
    removeFromNode(rightChild, 0);
    writeNode(this->indexHandler, rightChild, parent->children[underPos + 1]);

    free(rightChild);
    return 1;
}

void mergeLeft(TreeHandler* this, Node* parent, Node* underFlowed, int underPos) {
    Node* sibling = readNode(this->indexHandler, parent->children[underPos-1]);

    addToLeft(underFlowed,0,parent->keys[underPos-1],parent->ref[underPos-1],sibling->children[sibling->size]);

    for (int i = sibling->size-1; i >= 0; i--) {
        addToLeft(underFlowed,0,sibling->keys[i], sibling->ref[i],sibling->children[i]);
    }
    removeNode(this->indexHandler,parent->children[underPos-1]);
    removeFromNode(parent,underPos-1);
    free(sibling);
}

void mergeRight(TreeHandler* this, Node* parent, Node* underFlowed, int underPos) {
    Node* sibling = readNode(this->indexHandler, parent->children[underPos+1]);

    addToRight(underFlowed,underFlowed->size,parent->keys[underPos],parent->ref[underPos],sibling->children[0]);

    for (int i = 0; i < sibling->size; i++) {
        underFlowed->keys[underFlowed->size] = sibling->keys[i];
        underFlowed->ref[underFlowed->size] = sibling->ref[i];
        underFlowed->children[underFlowed->size+1] = sibling->children[i+1];
        underFlowed->size++;
    }
    removeNode(this->indexHandler,parent->children[underPos+1]);
    removeFromNode(parent,underPos);
    free(sibling);
}

Node *aux_remove(TreeHandler *this, int key, int nodePos) {
    Node *node = readNode(this->indexHandler, nodePos);
    int keyIndex;
    int find = searchKeyPosition(node, key, &keyIndex);

    if (find && isLeaf(node)) {
        removeFromNode(node, keyIndex);
    } else {
        if (find) {
            getMaxKeyAndRef(this, node->children[keyIndex], &node->keys[keyIndex], &node->ref[keyIndex]);
            key = node->keys[keyIndex];
        }
        Node *child = aux_remove(this, key, node->children[keyIndex]);

        if (isUnderflowed(child)) {
            int success = borrowLeft(this, node, child, keyIndex);
            if (!success)
                success = borrowRight(this, node, child, keyIndex);
            if (!success) {
                if (keyIndex < node->size)
                    mergeRight(this,node,child,keyIndex);
                else {
                    mergeLeft(this,node,child,keyIndex);
                    keyIndex--;
                }
            }
        }
        writeNode(this->indexHandler, child, node->children[keyIndex]);
        free(child);
    }
    return node;
}

void removeKey(TreeHandler *this, int key) {
    IndexHeader *header = getIndexHeader(this->indexHandler);

    if (header->root != -1) {
        Node *node = aux_remove(this, key, header->root);
        if (node->size == 0) {
            removeNode(this->indexHandler,header->root);
            header->root = node->children[0];
            writeIndexHeader(this->indexHandler);
        } else {
            writeNode(this->indexHandler, node, header->root);
        }
    }
}