#include "TreeHandler.h"
#include <stdlib.h>

// Cria um manipulador de arvore B recebendo um manipulador de indices
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Manipulador de aruvore B retornado
TreeHandler *createTreeHandler(IndexHandler* indexHandler) {
    TreeHandler *treeController = malloc(sizeof(TreeHandler));
    treeController->indexHandler = indexHandler;
    return treeController;
}

// Cria um nó vazio
// Pré-condição: Nenhuma
// Pós-condição: Ponteiro para nó sem filhos e com tamanho 0;
Node *createNode() {
    Node *node = malloc(sizeof(Node));
    node->size = 0;
    for (int i = 0; i < DEGREE + 1; i++) {
        node->children[i] = -1;
    }
    return node;
}

// Veritica se o nó fornecido é folha
// Pré-condição: Ponteiro para manipulador de indices válido
// Pós-condição: Nenhuma
int isLeaf(Node *node) {
    return node->children[0] == -1;
}

// Verifica se o nó fornecido esta em overflow
// Pré-condição: Ponteiro para nó válido
// Pós-condição: Nenhuma
int isOverflowed(Node *node) {
    return node->size == DEGREE;
}

// Verifica se o nó fornecido esta em underflow
// Pré-condição: Ponteiro para nó válido
// Pós-condição: Nenhuma
int isUnderflowed(Node *node) {
    return node->size < DEGREE / 2;
}

// Função recursiva para percorrer a arvore em ordem adicionando suas referencias a fila
// Pré-condição: Ponteiro para manipulador de arvore e fila válidos
// Pós-condição: Fila preenchida com referencias em ordem
static void aux_getReferencesAsc(TreeHandler *this, Queue *queue, int nodePos) {
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

// Fornece as referencias em ordem crescente
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Retorna uma fila contendo as referencias
Queue* getReferencesAsc(TreeHandler *this) {
    Queue *ascRefs = newQueue();
    IndexHeader *header = getIndexHeader(this->indexHandler);
    aux_getReferencesAsc(this, ascRefs, header->root);
    return ascRefs;
}

// Função recursiva para encontrar a referencia da chave passada
// Pré-condição: Ponteiro para manipulador de arvore
// Pós-condição: Retorna o numero da referencia da chave caso seja encontrada,
// caso contrario, -1 é retornado
static int aux_search(TreeHandler *this, int filePos, int key, int *pos) {
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

// Procura a referencia da chave fornecida na arvore B
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Retorna a referencia da chave, caso não seja encontrada,
// -1 é retornado
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

// Busca a posicao em que a chave esta ou estaria no nó
// Pré-condição: Ponteiro para nó valido
// Pós-condição: Retorna 1 caso chave esteja no nó e 0 caso contrario
int searchKeyPosition(Node *node, int key, int *pos) {
    for ((*pos) = 0; (*pos) < node->size; (*pos)++) {
        if (key == node->keys[(*pos)])
            return 1;
        else if (key < node->keys[(*pos)])
            break;
    }
    return 0;
}

// Adiciona a chave e referencia posição fornecida no nó fornecido, adicionando
// o filho fornecio a direita
// Pré-condição: Ponteiro para nó valido
// Pós-condição: Nó modificado
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

// Adiciona a chave e referencia posição fornecida no nó fornecido, adicionando
// o filho fornecio a esquerda
// Pré-condição: Ponteiro para nó valido
// Pós-condição: Nó modificado
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

// Cria novo nó a partir de metade das chaves e referencias do nó fornecido
// Pré-condição: Ponteiro para nó valido
// Pós-condição: Modifica o nó em overflow e retorna a chave e referencia mediana
// e o novo nó
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

// Função recursiva para inserir chave na arvore B, busca a posicao de insercao e
// corrige nós em overflow
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Retorna o nó da posição fornecida com as modificações necessárias
Node *aux_insert(TreeHandler *this, int nodePos, int key, int ref) {
    int pos;
    Node *node = readNode(this->indexHandler, nodePos);

    if (!searchKeyPosition(node, key, &pos)) {
        if (isLeaf(node)) {
            addToRight(node, pos, key, ref, -1);
        } else {
            Node *aux = aux_insert(this, node->children[pos], key, ref);
            if (isOverflowed(aux)) {
                int medKey, medRef;
                Node *newNode = split(aux, &medKey, &medRef);
                int newRef = addNode(this->indexHandler, newNode);
                addToRight(node, pos, medKey, medRef, newRef);
                writeNode(this->indexHandler, aux,node->children[pos]);
                free(newNode);
            }
            writeNode(this->indexHandler, aux, node->children[pos]);
            free(aux);
        }
    }
    return node;
}

// Insere uma chave e uma referencia na arvore B realizando as devidas
// correçoes caso o nó fiquem em overflow
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Chave e referencia adicionadas
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
        node = aux_insert(this, header->root, key, ref);
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
            int rootPos = addNode(this->indexHandler, newRoot);
            writeNode(this->indexHandler, node, header->root);
            header->root = rootPos;
            writeIndexHeader(this->indexHandler);
        } else {
            writeNode(this->indexHandler, node, header->root);
        }
    }
    free(node);
}

// Remove a chave e referencia na posição fornecida do nó fornecido
// Pré-condição: Ponteiro para nó valido
// Pós-condição: Nó modificado
void removeFromNode(Node *node, int pos) {
    for (int i = pos; i < node->size; i++) {
        node->keys[i] = node->keys[i + 1];
        node->ref[i] = node->ref[i + 1];
        node->children[i + 1] = node->children[i + 2];
    }
    node->size--;
}

// Funçao recursiva para encontrar a maior chave e referencia a partir da posiçao
// no arquivo de um no
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Nenhuma
void getMaxKeyAndRef(TreeHandler *this, int nodePos, int *key, int *ref) {
    Node *node = readNode(this->indexHandler, nodePos);

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

// Empresta uma chave do no esquerdo ao no em underflow
// Pré-condição: Ponteiro para manipulador de arvore e nos válidos
// Pós-condição: Retorna 1 caso obteha sucesso e 0 caso contrario
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

// Empresta uma chave do no direito ao no em underflow
// Pré-condição: Ponteiro para manipulador de arvore e nos válidos
// Pós-condição: Retorna 1 caso obteha sucesso e 0 caso contrario
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

// Adiciona chaves e referencias do no adjacente esquerdo e do no pai ao
// no em underflow, removendo o no adjacente esquerdo
// Pré-condição: Ponteiro para manipulador de arvore e nos válidos
// Pós-condição: No adjacente esquerdo removido e nos com underflow e pai modificados
void mergeLeft(TreeHandler* this, Node* parent, Node* underFlowed, int underPos) {
    Node* leftChild = readNode(this->indexHandler, parent->children[underPos - 1]);

    addToLeft(underFlowed, 0, parent->keys[underPos-1], parent->ref[underPos-1], leftChild->children[leftChild->size]);

    for (int i = leftChild->size - 1; i >= 0; i--) {
        addToLeft(underFlowed, 0, leftChild->keys[i], leftChild->ref[i], leftChild->children[i]);
    }
    removeNode(this->indexHandler, parent->children[underPos-1]);
    parent->children[underPos-1] = parent->children[underPos];
    removeFromNode(parent,underPos-1);
    free(leftChild);
}

// Adiciona chaves e referencias do no adjacente direito e do no pai ao
// no em underflow, removendo o no adjacente direito
// Pré-condição: Ponteiro para manipulador de arvore e nos válidos
// Pós-condição: No adjacente direito removido e nos com underflow e pai modificados
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

// Função recursiva para remover chave na arvore B, busca a posicao de remocao e
// corrige nós em underflow
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Retorna o nó da posição fornecida com as modificações necessárias
Node* aux_remove(TreeHandler *this, int key, int nodePos) {
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

// Remove uma chave da arvore B realizando as devidas correçoes
// caso o nó fique em underflow
// Pré-condição: Ponteiro para manipulador de arvore válido
// Pós-condição: Chave removida da arvore
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