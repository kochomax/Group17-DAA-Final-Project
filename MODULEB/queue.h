queue.h                                                                                                                    #ifndef QUEUE_H
#define QUEUE_H

typedef struct
{
    int *items;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

int queueInit(Queue *queue, int capacity);
void queueFree(Queue *queue);
int enqueue(Queue *queue, int value);
int dequeue(Queue *queue, int *value);
int queueFront(Queue *queue, int *value);
int queueIsEmpty(Queue *queue);
int queueIsFull(Queue *queue);

#endif
