#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// =========================================
// SINGLY LINKED LIST
// =========================================

typedef struct SinglyNode
{
    int data;
    struct SinglyNode *next;
} SinglyNode;

typedef struct
{
    SinglyNode *head;
    int size;
} SinglyList;

// Singly linked list operations
void singlyInit(SinglyList *list);
void singlyClear(SinglyList *list);

int singlyInsertBeginning(SinglyList *list, int value);
int singlyInsertEnd(SinglyList *list, int value);
int singlyInsertBefore(SinglyList *list, int target, int value);
int singlyInsertAfter(SinglyList *list, int target, int value);

int singlyDeleteBeginning(SinglyList *list, int *removed);
int singlyDeleteEnd(SinglyList *list, int *removed);
int singlyDeleteValue(SinglyList *list, int value);
int singlyDeleteBefore(SinglyList *list, int target, int *removed);
int singlyDeleteAfter(SinglyList *list, int target, int *removed);

int singlySearch(const SinglyList *list, int value);
int singlyGetAt(const SinglyList *list, int index, int *value);


// =========================================
// DOUBLY LINKED LIST
// =========================================

typedef struct DoublyNode
{
    int data;
    struct DoublyNode *prev;
    struct DoublyNode *next;
} DoublyNode;

typedef struct
{
    DoublyNode *head;
    DoublyNode *tail;
    int size;
} DoublyList;

// Doubly linked list operations
void doublyInit(DoublyList *list);
void doublyClear(DoublyList *list);

int doublyInsertBeginning(DoublyList *list, int value);
int doublyInsertEnd(DoublyList *list, int value);
int doublyInsertBefore(DoublyList *list, int target, int value);
int doublyInsertAfter(DoublyList *list, int target, int value);

int doublyDeleteBeginning(DoublyList *list, int *removed);
int doublyDeleteEnd(DoublyList *list, int *removed);
int doublyDeleteValue(DoublyList *list, int value);
int doublyDeleteBefore(DoublyList *list, int target, int *removed);
int doublyDeleteAfter(DoublyList *list, int target, int *removed);

int doublySearch(const DoublyList *list, int value);
int doublyGetForwardAt(const DoublyList *list, int index, int *value);
int doublyGetBackwardAt(const DoublyList *list, int index, int *value);

#endif