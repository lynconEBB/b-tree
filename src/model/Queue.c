#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue *newQueue()
{
    Queue* queue = malloc(sizeof(Queue));
    queue->n = 0;
    queue->last = NULL;
    queue->first = NULL;
    return queue;
}

int isEmpty(Queue *q)
{
    return (q->n <= 0);
}

void printElements(Queue *q)
{
    printf("[ ");
    QueueNode* curr = q->first;
    while (curr != NULL) {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("]\n");
}

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