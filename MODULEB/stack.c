#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Initialize the stack
int stackInit(Stack *stack, int capacity)
{
    if (capacity <= 0)
    {
        return 0;
    }

    stack->items = malloc(capacity * sizeof(int));

    if (stack->items == NULL)
    {
        return 0;
    }

    stack->top = -1;
    stack->capacity = capacity;

    return 1;
}

// Free the memory used by the stack
void stackFree(Stack *stack)
{
    free(stack->items);

    stack->items = NULL;
    stack->top = -1;
    stack->capacity = 0;
}

// Push a value onto the stack
int stackPush(Stack *stack, int value)
{
    if (stackIsFull(stack))
    {
        return 0;
    }

    stack->top++;
    stack->items[stack->top] = value;

    return 1;
}

// Remove the top value from the stack
int stackPop(Stack *stack, int *value)
{
    if (stackIsEmpty(stack))
    {
        return 0;
    }

    *value = stack->items[stack->top];
    stack->top--;

    return 1;
}

// View the top value without removing it
int stackPeek(Stack *stack, int *value)
{
    if (stackIsEmpty(stack))
    {
        return 0;
    }

    *value = stack->items[stack->top];

    return 1;
}

// Get the number of elements in the stack
int stackSize(Stack *stack)
{
    return stack->top + 1;
}

// Check whether the stack is empty
int stackIsEmpty(Stack *stack)
{
    return stack->top == -1;
}

// Check whether the stack is full
int stackIsFull(Stack *stack)
{
    return stack->top == stack->capacity - 1;
}
