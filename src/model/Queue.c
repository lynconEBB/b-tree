#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>

// Cria fila vazia
// Pré-condição: Nenhuma
// Pós-condição: Nova fila retornada;
Queue *newQueue()
{
    Queue* queue = malloc(sizeof(Queue));
    queue->n = 0;
    queue->last = NULL;
    queue->first = NULL;
    return queue;
}

// Verifica se a final fornecida é vazia ou não
// Pré-condição: Ponteiro para fila valida
// Pós-condição: Retorna 1 se a fila estiver vazia e 0 caso contrários
int isEmpty(Queue *q)
{
    return (q->n <= 0);
}

// Adiciona elemnto fornecido ao final da fila fornecida
// Pré-condição: Ponteiro para fila valida
// Pós-condição: Elemento adicionado ao final da final
void enqueue(Queue *q, int data)
{
    QueueNode * aux = malloc(sizeof(QueueNode));
    aux->data = data;
    aux->next = NULL;

    if (isEmpty(q)) {
        q->first = aux;
    } else {
        q->last->next = aux;
    }
    q->n++;
    q->last = aux;
}

// Retira primeiro elemento da fila
// Pré-condição: Ponteiro para fila valida
// Pós-condição: Primeiro elemento removido caso exista
void dequeue(Queue *q)
{
    if (isEmpty(q)) {
        return;
    }
    QueueNode* toBeDeleted = q->first;
    if (q->n == 1)
        q->last = NULL;

    q->first = q->first->next;
    q->n--;
    free(toBeDeleted);
}