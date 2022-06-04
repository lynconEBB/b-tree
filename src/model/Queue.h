#ifndef BTREE_QUEUE_H
#define BTREE_QUEUE_H

typedef struct node {
    int data;
    struct node* next;
} QueueNode;

typedef struct {
    int n;
    QueueNode* first;
    QueueNode* last;
} Queue;

Queue* newQueue();

int isEmpty(Queue* q);

void enqueue(Queue* q, int data);

void dequeue(Queue* q);

void printElements(Queue* q);

#endif
