#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct SNode{ int data; struct SNode *next; } SNode;
typedef struct { SNode *head; int size; } SList;

typedef struct DNode{ int data; struct DNode *next; struct DNode *prev; } DNode;
typedef struct { DNode *head; DNode *tail; int size; } DList;

void s_init(SList *l);
void s_insert_begin(SList *l,int v);
void s_insert_end(SList *l,int v);
int s_insert_after(SList *l,int target,int v);
int s_insert_before(SList *l,int target,int v);
int s_delete_begin(SList *l,int *out);
int s_delete_end(SList *l,int *out);
int s_delete_value(SList *l,int v);
int s_delete_after(SList *l,int target,int *out);
int s_delete_before(SList *l,int target,int *out);
int s_search(SList *l,int v);
void s_display(SList *l);

void d_init(DList *l);
void d_insert_begin(DList *l,int v);
void d_insert_end(DList *l,int v);
int d_insert_after(DList *l,int target,int v);
int d_insert_before(DList *l,int target,int v);
int d_delete_begin(DList *l,int *out);
int d_delete_end(DList *l,int *out);
int d_delete_value(DList *l,int v);
int d_delete_after(DList *l,int target,int *out);
int d_delete_before(DList *l,int target,int *out);
int d_search(DList *l,int v);
void d_display_forward(DList *l);
void d_display_backward(DList *l);

#endif
