#include <stdlib.h>
#include "linked_list.h"

// =========================================
// SINGLY LINKED LIST
// =========================================

// Initialize singly linked list
void singlyInit(SinglyList *list)
{
    list->head = NULL;
    list->size = 0;
}

// Clear singly linked list
void singlyClear(SinglyList *list)
{
    SinglyNode *current = list->head;

    while (current != NULL)
    {
        SinglyNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->size = 0;
}

// Create a new singly linked node
static SinglyNode *newSinglyNode(int value)
{
    SinglyNode *node = malloc(sizeof(SinglyNode));

    if (node == NULL)
        return NULL;

    node->data = value;
    node->next = NULL;

    return node;
}

// Insert at beginning
int singlyInsertBeginning(SinglyList *list, int value)
{
    SinglyNode *node = newSinglyNode(value);

    if (node == NULL)
        return 0;

    node->next = list->head;
    list->head = node;

    list->size++;

    return 1;
}

// Insert at end
int singlyInsertEnd(SinglyList *list, int value)
{
    SinglyNode *node = newSinglyNode(value);

    if (node == NULL)
        return 0;

    if (list->head == NULL)
    {
        list->head = node;
    }
    else
    {
        SinglyNode *current = list->head;

        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = node;
    }

    list->size++;

    return 1;
}

// Insert before a given value
int singlyInsertBefore(SinglyList *list, int target, int value)
{
    if (list->head == NULL)
        return 0;

    if (list->head->data == target)
    {
        return singlyInsertBeginning(list, value);
    }

    SinglyNode *current = list->head;

    while (current->next != NULL &&
           current->next->data != target)
    {
        current = current->next;
    }

    if (current->next == NULL)
        return 0;

    SinglyNode *node = newSinglyNode(value);

    if (node == NULL)
        return 0;

    node->next = current->next;
    current->next = node;

    list->size++;

    return 1;
}

// Insert after a given value
int singlyInsertAfter(SinglyList *list, int target, int value)
{
    SinglyNode *current = list->head;

    while (current != NULL && current->data != target)
    {
        current = current->next;
    }

    if (current == NULL)
        return 0;

    SinglyNode *node = newSinglyNode(value);

    if (node == NULL)
        return 0;

    node->next = current->next;
    current->next = node;

    list->size++;

    return 1;
}

// Delete from beginning
int singlyDeleteBeginning(SinglyList *list, int *removed)
{
    if (list->head == NULL)
        return 0;

    SinglyNode *node = list->head;

    if (removed != NULL)
        *removed = node->data;

    list->head = node->next;

    free(node);

    list->size--;

    return 1;
}

// Delete from end
int singlyDeleteEnd(SinglyList *list, int *removed)
{
    if (list->head == NULL)
        return 0;

    if (list->head->next == NULL)
    {
        return singlyDeleteBeginning(list, removed);
    }

    SinglyNode *current = list->head;

    while (current->next->next != NULL)
    {
        current = current->next;
    }

    if (removed != NULL)
        *removed = current->next->data;

    free(current->next);
    current->next = NULL;

    list->size--;

    return 1;
}

// Delete a given value
int singlyDeleteValue(SinglyList *list, int value)
{
    if (list->head == NULL)
        return 0;

    if (list->head->data == value)
    {
        return singlyDeleteBeginning(list, NULL);
    }

    SinglyNode *current = list->head;

    while (current->next != NULL &&
           current->next->data != value)
    {
        current = current->next;
    }

    if (current->next == NULL)
        return 0;

    SinglyNode *node = current->next;

    current->next = node->next;

    free(node);

    list->size--;

    return 1;
}

// Delete the node immediately before a given value
int singlyDeleteBefore(SinglyList *list, int target, int *removed)
{
    if (list->head == NULL || list->head->data == target)
        return 0;

    SinglyNode *previousPrevious = NULL;
    SinglyNode *previous = list->head;
    SinglyNode *current = list->head->next;

    while (current != NULL && current->data != target)
    {
        previousPrevious = previous;
        previous = current;
        current = current->next;
    }

    if (current == NULL)
        return 0;

    if (removed != NULL)
        *removed = previous->data;

    if (previousPrevious == NULL)
    {
        list->head = current;
    }
    else
    {
        previousPrevious->next = current;
    }

    free(previous);

    list->size--;

    return 1;
}

// Delete the node immediately after a given value
int singlyDeleteAfter(SinglyList *list, int target, int *removed)
{
    SinglyNode *current = list->head;

    while (current != NULL && current->data != target)
    {
        current = current->next;
    }

    if (current == NULL || current->next == NULL)
        return 0;

    SinglyNode *node = current->next;

    if (removed != NULL)
        *removed = node->data;

    current->next = node->next;

    free(node);

    list->size--;

    return 1;
}

// Search for a value
int singlySearch(const SinglyList *list, int value)
{
    int index = 0;

    for (SinglyNode *current = list->head;
         current != NULL;
         current = current->next)
    {
        if (current->data == value)
            return index;

        index++;
    }

    return -1;
}

// Retrieve a value using its position
int singlyGetAt(const SinglyList *list, int index, int *value)
{
    if (index < 0 || index >= list->size || value == NULL)
        return 0;

    SinglyNode *current = list->head;

    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    *value = current->data;

    return 1;
}


// =========================================
// DOUBLY LINKED LIST
// =========================================

// Initialize doubly linked list
void doublyInit(DoublyList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Clear doubly linked list
void doublyClear(DoublyList *list)
{
    DoublyNode *current = list->head;

    while (current != NULL)
    {
        DoublyNode *next = current->next;

        free(current);

        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Create a new doubly linked node
static DoublyNode *newDoublyNode(int value)
{
    DoublyNode *node = malloc(sizeof(DoublyNode));

    if (node == NULL)
        return NULL;

    node->data = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

// Insert at beginning
int doublyInsertBeginning(DoublyList *list, int value)
{
    DoublyNode *node = newDoublyNode(value);

    if (node == NULL)
        return 0;

    node->next = list->head;

    if (list->head != NULL)
    {
        list->head->prev = node;
    }
    else
    {
        list->tail = node;
    }

    list->head = node;

    list->size++;

    return 1;
}

// Insert at end
int doublyInsertEnd(DoublyList *list, int value)
{
    DoublyNode *node = newDoublyNode(value);

    if (node == NULL)
        return 0;

    node->prev = list->tail;

    if (list->tail != NULL)
    {
        list->tail->next = node;
    }
    else
    {
        list->head = node;
    }

    list->tail = node;

    list->size++;

    return 1;
}

// Insert before a given value
int doublyInsertBefore(DoublyList *list, int target, int value)
{
    DoublyNode *current = list->head;

    while (current != NULL && current->data != target)
    {
        current = current->next;
    }

    if (current == NULL)
        return 0;

    if (current == list->head)
    {
        return doublyInsertBeginning(list, value);
    }

    DoublyNode *node = newDoublyNode(value);

    if (node == NULL)
        return 0;

    node->prev = current->prev;
    node->next = current;

    current->prev->next = node;
    current->prev = node;

    list->size++;

    return 1;
}

// Insert after a given value
int doublyInsertAfter(DoublyList *list, int target, int value)
{
    DoublyNode *current = list->head;

    while (current != NULL && current->data != target)
    {
        current = current->next;
    }

    if (current == NULL)
        return 0;

    if (current == list->tail)
    {
        return doublyInsertEnd(list, value);
    }

    DoublyNode *node = newDoublyNode(value);

    if (node == NULL)
        return 0;

    node->next = current->next;
    node->prev = current;

    current->next->prev = node;
    current->next = node;

    list->size++;

    return 1;
}

// Common helper to remove a doubly linked node
static void unlinkDoublyNode(
    DoublyList *list,
    DoublyNode *node,
    int *removed
)
{
    if (removed != NULL)
        *removed = node->data;

    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else
    {
        list->head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    else
    {
        list->tail = node->prev;
    }

    free(node);

    list->size--;
}

// Delete from beginning
int doublyDeleteBeginning(DoublyList *list, int *removed)
{
    if (list->head == NULL)
        return 0;

    unlinkDoublyNode(list, list->head, removed);

    return 1;
}

// Delete from end
int doublyDeleteEnd(DoublyList *list, int *removed)
{
    if (list->tail == NULL)
        return 0;

    unlinkDoublyNode(list, list->tail, removed);

    return 1;
}

// Delete a given value
int doublyDeleteValue(DoublyList *list, int value)
{
    DoublyNode *current = list->head;

    while (current != NULL && current->data != value)
    {
        current = current->next;
    }

    if (current == NULL)
        return 0;

    unlinkDoublyNode(list, current, NULL);

    return 1;
}

// Delete the node immediately before a given value
int doublyDeleteBefore(DoublyList *list, int target, int *removed)
{
    DoublyNode *current = list->head;

    while (current != NULL && current->data != target)
    {
        current = current->next;
    }

    if (current == NULL || current->prev == NULL)
        return 0;

    unlinkDoublyNode(list, current->prev, removed);

    return 1;
}

// Delete the node immediately after a given value
int doublyDeleteAfter(DoublyList *list, int target, int *removed)
{
    DoublyNode *current = list->head;

    while (current != NULL && current->data != target)
    {
        current = current->next;
    }

    if (current == NULL || current->next == NULL)
        return 0;

    unlinkDoublyNode(list, current->next, removed);

    return 1;
}

// Search for a value
int doublySearch(const DoublyList *list, int value)
{
    int index = 0;

    for (DoublyNode *current = list->head;
         current != NULL;
         current = current->next)
    {
        if (current->data == value)
            return index;

        index++;
    }

    return -1;
}

// Retrieve a value from the front
int doublyGetForwardAt(
    const DoublyList *list,
    int index,
    int *value
)
{
    if (index < 0 || index >= list->size || value == NULL)
        return 0;

    DoublyNode *current = list->head;

    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    *value = current->data;

    return 1;
}

// Retrieve a value from the back
// Index 0 represents the last node
int doublyGetBackwardAt(
    const DoublyList *list,
    int index,
    int *value
)
{
    if (index < 0 || index >= list->size || value == NULL)
        return 0;

    DoublyNode *current = list->tail;

    for (int i = 0; i < index; i++)
    {
        current = current->prev;
    }

    *value = current->data;

    return 1;
}