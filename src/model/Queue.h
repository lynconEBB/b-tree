#ifndef BTREE_QUEUE_H
#define BTREE_QUEUE_H

// Estrutura de nó de uma fila
typedef struct node {
    int data;
    struct node* next;
} QueueNode;

// Estrutura de uma fila
typedef struct {
    int n;
    QueueNode* first;
    QueueNode* last;
} Queue;

// Cria fila vazia
// Pré-condição: Nenhuma
// Pós-condição: Nova fila retornada;
Queue* newQueue();

// Verifica se a final fornecida é vazia ou não
// Pré-condição: Ponteiro para fila valida
// Pós-condição: Retorna 1 se a fila estiver vazia e 0 caso contrários
int isEmpty(Queue* q);

// Adiciona elemnto fornecido ao final da fila fornecida
// Pré-condição: Ponteiro para fila valida
// Pós-condição: Elemento adicionado ao final da final
void enqueue(Queue* q, int data);

// Retira primeiro elemento da fila
// Pré-condição: Ponteiro para fila valida
// Pós-condição: Primeiro elemento removido caso exista
void dequeue(Queue* q);

#endif