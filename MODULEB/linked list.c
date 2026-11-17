#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

// --- SINGLY ---
void s_init(SList *l){ l->head=NULL; l->size=0; }
void s_insert_begin(SList *l,int v){ SNode *n=malloc(sizeof(SNode)); n->data=v; n->next=l->head; l->head=n; l->size++; }
void s_insert_end(SList *l,int v){ SNode *n=malloc(sizeof(SNode)); n->data=v; n->next=NULL; if(!l->head){l->head=n;} else { SNode *t=l->head; while(t->next) t=t->next; t->next=n; } l->size++; }
int s_insert_after(SList *l,int target,int v){ SNode *t=l->head; while(t && t->data!=target) t=t->next; if(!t) return -1; SNode *n=malloc(sizeof(SNode)); n->data=v; n->next=t->next; t->next=n; l->size++; return 0; }
int s_insert_before(SList *l,int target,int v){ if(!l->head) return -1; if(l->head->data==target){ s_insert_begin(l,v); return 0; } SNode *t=l->head; while(t->next && t->next->data!=target) t=t->next; if(!t->next) return -1; SNode *n=malloc(sizeof(SNode)); n->data=v; n->next=t->next; t->next=n; l->size++; return 0; }
int s_delete_begin(SList *l,int *out){ if(!l->head) return -1; SNode *t=l->head; *out=t->data; l->head=t->next; free(t); l->size--; return 0; }
int s_delete_end(SList *l,int *out){ if(!l->head) return -1; if(!l->head->next) return s_delete_begin(l,out); SNode *t=l->head; while(t->next->next) t=t->next; *out=t->next->data; free(t->next); t->next=NULL; l->size--; return 0; }
int s_delete_value(SList *l,int v){ if(!l->head) return -1; if(l->head->data==v) return s_delete_begin(l,&v); SNode *t=l->head; while(t->next && t->next->data!=v) t=t->next; if(!t->next) return -1; SNode *d=t->next; t->next=d->next; free(d); l->size--; return 0; }
int s_delete_after(SList *l,int target,int *out){ SNode *t=l->head; while(t && t->data!=target) t=t->next; if(!t || !t->next) return -1; SNode *d=t->next; *out=d->data; t->next=d->next; free(d); l->size--; return 0; }
int s_delete_before(SList *l,int target,int *out){ if(!l->head || l->head->data==target || !l->head->next) return -1; if(l->head->next->data==target) return s_delete_begin(l,out); SNode *t=l->head; while(t->next->next && t->next->next->data!=target) t=t->next; if(!t->next->next) return -1; SNode *d=t->next; *out=d->data; t->next=d->next; free(d); l->size--; return 0; }
int s_search(SList *l,int v){ SNode *t=l->head; int pos=0; while(t){ if(t->data==v) return pos; t=t->next; pos++; } return -1; }
void s_display(SList *l){ if(!l->head){ printf("Singly List: [empty]\n"); return; } printf("Singly List: "); for(SNode *t=l->head;t;t=t->next) printf("%d -> ",t->data); printf("NULL (size=%d)\n",l->size); }

// --- DOUBLY ---
void d_init(DList *l){ l->head=NULL; l->tail=NULL; l->size=0; }
void d_insert_begin(DList *l,int v){ DNode *n=malloc(sizeof(DNode)); n->data=v; n->prev=NULL; n->next=l->head; if(l->head) l->head->prev=n; else l->tail=n; l->head=n; l->size++; }
void d_insert_end(DList *l,int v){ DNode *n=malloc(sizeof(DNode)); n->data=v; n->next=NULL; n->prev=l->tail; if(l->tail) l->tail->next=n; else l->head=n; l->tail=n; l->size++; }
int d_insert_after(DList *l,int target,int v){ DNode *t=l->head; while(t && t->data!=target) t=t->next; if(!t) return -1; DNode *n=malloc(sizeof(DNode)); n->data=v; n->next=t->next; n->prev=t; if(t->next) t->next->prev=n; else l->tail=n; t->next=n; l->size++; return 0; }
int d_insert_before(DList *l,int target,int v){ DNode *t=l->head; while(t && t->data!=target) t=t->next; if(!t) return -1; DNode *n=malloc(sizeof(DNode)); n->data=v; n->prev=t->prev; n->next=t; if(t->prev) t->prev->next=n; else l->head=n; t->prev=n; l->size++; return 0; }
int d_delete_begin(DList *l,int *out){ if(!l->head) return -1; DNode *t=l->head; *out=t->data; l->head=t->next; if(l->head) l->head->prev=NULL; else l->tail=NULL; free(t); l->size--; return 0; }
int d_delete_end(DList *l,int *out){ if(!l->tail) return -1; DNode *t=l->tail; *out=t->data; l->tail=t->prev; if(l->tail) l->tail->next=NULL; else l->head=NULL; free(t); l->size--; return 0; }
int d_delete_value(DList *l,int v){ DNode *t=l->head; while(t && t->data!=v) t=t->next; if(!t) return -1; if(t->prev) t->prev->next=t->next; else l->head=t->next; if(t->next) t->next->prev=t->prev; else l->tail=t->prev; free(t); l->size--; return 0; }
int d_delete_after(DList *l,int target,int *out){ DNode *t=l->head; while(t && t->data!=target) t=t->next; if(!t || !t->next) return -1; DNode *d=t->next; *out=d->data; t->next=d->next; if(d->next) d->next->prev=t; else l->tail=t; free(d); l->size--; return 0; }
int d_delete_before(DList *l,int target,int *out){ DNode *t=l->head; while(t && t->data!=target) t=t->next; if(!t || !t->prev) return -1; DNode *d=t->prev; *out=d->data; if(d->prev) d->prev->next=t; else l->head=t; t->prev=d->prev; free(d); l->size--; return 0; }
int d_search(DList *l,int v){ DNode *t=l->head; int pos=0; while(t){ if(t->data==v) return pos; t=t->next; pos++; } return -1; }
void d_display_forward(DList *l){ if(!l->head){ printf("Doubly List: [empty]\n"); return; } printf("Doubly Forward: "); for(DNode *t=l->head;t;t=t->next) printf("%d <-> ",t->data); printf("NULL (size=%d)\n",l->size); }
void d_display_backward(DList *l){ if(!l->tail){ printf("Doubly Backward: [empty]\n"); return; } printf("Doubly Backward: "); for(DNode *t=l->tail;t;t=t->prev) printf("%d <-> ",t->data); printf("NULL\n"); }
