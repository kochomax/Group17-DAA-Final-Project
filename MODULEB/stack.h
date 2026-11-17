#ifndef STACK_H
#define STACK_H

typedef struct
{
    int *items;
    int top;
    int capacity;
} Stack;

int stackInit(Stack *stack, int capacity);
void stackFree(Stack *stack);
int stackPush(Stack *stack, int value);
int stackPop(Stack *stack, int *value);
int stackPeek(Stack *stack, int *value);
int stackSize(Stack *stack);
int stackIsEmpty(Stack *stack);
int stackIsFull(Stack *stack);

#endif
