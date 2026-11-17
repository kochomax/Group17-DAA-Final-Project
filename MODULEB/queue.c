queue.c                                                                                                           #include <stdlib.h>
#include "queue.h"

int queueInit(Queue *queue, int capacity)
{
    if (capacity <= 0) return 0;

    queue->items = malloc(capacity * sizeof(int));

    if (queue->items == NULL) return 0;

    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = capacity;

    return 1;
}

void queueFree(Queue *queue)
{
    free(queue->items);
    queue->items = NULL;
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = 0;
}

int enqueue(Queue *queue, int value)
{
    if (queueIsFull(queue)) return 0;

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->items[queue->rear] = value;
    queue->size++;

    return 1;
}

int dequeue(Queue *queue, int *value)
{
    if (queueIsEmpty(queue)) return 0;

    *value = queue->items[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    if (queue->size == 0)
    {
        queue->front = 0;
        queue->rear = -1;
    }

    return 1;
}

int queueFront(Queue *queue, int *value)
{
    if (queueIsEmpty(queue)) return 0;

    *value = queue->items[queue->front];
    return 1;
}

int queueIsEmpty(Queue *queue)
{
    return queue->size == 0;
}

int queueIsFull(Queue *queue)
{
    return queue->size == queue->capacity;
}
