#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/stack.h"
#include "src/queue.h"
#include "src/linked_list.h"
#include "src/bracket_checker.h"

void stack_menu(){
    int cap; printf("Enter stack capacity: "); scanf("%d",&cap);
    Stack s; stack_init(&s,cap);
    int choice,val,out;
    do{
        printf("\n--- STACK ---\n1.Push 2.Pop 3.Peek 4.Size 5.IsEmpty 6.IsFull 7.Display 0.Back\nChoice: ");
        scanf("%d",&choice);
        if(choice==1){ printf("Value: "); scanf("%d",&val); stack_push(&s,val); }
        else if(choice==2){ stack_pop(&s,&out); }
        else if(choice==3){ if(stack_peek(&s,&out)==0) printf("Top: %d\n",out); }
        else if(choice==4){ printf("Size: %d\n",stack_size(&s)); }
        else if(choice==5){ printf(stack_is_empty(&s)?"Empty\n":"Not empty\n"); }
        else if(choice==6){ printf(stack_is_full(&s)?"Full\n":"Not full\n"); }
        else if(choice==7){ stack_display(&s); }
    }while(choice!=0);
    stack_free(&s);
}

void queue_menu(){
    int cap; printf("Enter queue capacity: "); scanf("%d",&cap);
    LinearQueue lq; lq_init(&lq,cap);
    CircularQueue cq; cq_init(&cq,cap);
    int choice,type,val,out;
    do{
        printf("\n--- QUEUE ---\n1.Linear Enqueue 2.Linear Dequeue 3.Linear Front 4.Linear Display\n5.Circular Enqueue 6.Circular Dequeue 7.Circular Front 8.Circular Display 0.Back\nChoice: ");
        scanf("%d",&choice);
        if(choice==1){ printf("Value: "); scanf("%d",&val); lq_enqueue(&lq,val); }
        else if(choice==2){ lq_dequeue(&lq,&out); }
        else if(choice==3){ if(lq_front(&lq,&out)==0) printf("Front: %d\n",out); else printf("Empty\n"); }
        else if(choice==4){ lq_display(&lq); }
        else if(choice==5){ printf("Value: "); scanf("%d",&val); cq_enqueue(&cq,val); }
        else if(choice==6){ cq_dequeue(&cq,&out); }
        else if(choice==7){ if(cq_front(&cq,&out)==0) printf("Front: %d\n",out); else printf("Empty\n"); }
        else if(choice==8){ cq_display(&cq); }
    }while(choice!=0);
    lq_free(&lq); cq_free(&cq);
}

void sll_menu(){
    SList l; s_init(&l);
    int choice,val,target,out;
    do{
        printf("\n--- SINGLY LINKED LIST ---\n1.Insert Begin 2.Insert End 3.Insert After 4.Insert Before\n5.Delete Begin 6.Delete End 7.Delete Value 8.Delete After 9.Delete Before\n10.Search 11.Display 0.Back\nChoice: ");
        scanf("%d",&choice);
        if(choice==1){ printf("Value: "); scanf("%d",&val); s_insert_begin(&l,val); }
        else if(choice==2){ printf("Value: "); scanf("%d",&val); s_insert_end(&l,val); }
        else if(choice==3){ printf("After which? "); scanf("%d",&target); printf("Value: "); scanf("%d",&val); if(s_insert_after(&l,target,val)==-1) printf("Not found\n"); }
        else if(choice==4){ printf("Before which? "); scanf("%d",&target); printf("Value: "); scanf("%d",&val); if(s_insert_before(&l,target,val)==-1) printf("Not found\n"); }
        else if(choice==5){ if(s_delete_begin(&l,&out)==0) printf("Deleted %d\n",out); else printf("Empty\n"); }
        else if(choice==6){ if(s_delete_end(&l,&out)==0) printf("Deleted %d\n",out); else printf("Empty\n"); }
        else if(choice==7){ printf("Value to delete: "); scanf("%d",&val); if(s_delete_value(&l,val)==-1) printf("Not found\n"); else printf("Deleted %d\n",val); }
        else if(choice==8){ printf("After which? "); scanf("%d",&target); if(s_delete_after(&l,target,&out)==0) printf("Deleted %d\n",out); else printf("Not found\n"); }
        else if(choice==9){ printf("Before which? "); scanf("%d",&target); if(s_delete_before(&l,target,&out)==0) printf("Deleted %d\n",out); else printf("Not found\n"); }
        else if(choice==10){ printf("Search: "); scanf("%d",&val); int p=s_search(&l,val); if(p==-1) printf("Not found\n"); else printf("Found at position %d\n",p); }
        else if(choice==11){ s_display(&l); }
    }while(choice!=0);
}

void dll_menu(){
    DList l; d_init(&l);
    int choice,val,target,out;
    do{
        printf("\n--- DOUBLY LINKED LIST ---\n1.Insert Begin 2.Insert End 3.Insert After 4.Insert Before\n5.Delete Begin 6.Delete End 7.Delete Value 8.Delete After 9.Delete Before\n10.Search 11.Display Forward 12.Display Backward 0.Back\nChoice: ");
        scanf("%d",&choice);
        if(choice==1){ printf("Value: "); scanf("%d",&val); d_insert_begin(&l,val); }
        else if(choice==2){ printf("Value: "); scanf("%d",&val); d_insert_end(&l,val); }
        else if(choice==3){ printf("After which? "); scanf("%d",&target); printf("Value: "); scanf("%d",&val); if(d_insert_after(&l,target,val)==-1) printf("Not found\n"); }
        else if(choice==4){ printf("Before which? "); scanf("%d",&target); printf("Value: "); scanf("%d",&val); if(d_insert_before(&l,target,val)==-1) printf("Not found\n"); }
        else if(choice==5){ if(d_delete_begin(&l,&out)==0) printf("Deleted %d\n",out); else printf("Empty\n"); }
        else if(choice==6){ if(d_delete_end(&l,&out)==0) printf("Deleted %d\n",out); else printf("Empty\n"); }
        else if(choice==7){ printf("Value to delete: "); scanf("%d",&val); if(d_delete_value(&l,val)==-1) printf("Not found\n"); else printf("Deleted %d\n",val); }
        else if(choice==8){ printf("After which? "); scanf("%d",&target); if(d_delete_after(&l,target,&out)==0) printf("Deleted %d\n",out); else printf("Not found\n"); }
        else if(choice==9){ printf("Before which? "); scanf("%d",&target); if(d_delete_before(&l,target,&out)==0) printf("Deleted %d\n",out); else printf("Not found\n"); }
        else if(choice==10){ printf("Search: "); scanf("%d",&val); int p=d_search(&l,val); if(p==-1) printf("Not found\n"); else printf("Found at position %d\n",p); }
        else if(choice==11){ d_display_forward(&l); }
        else if(choice==12){ d_display_backward(&l); }
    }while(choice!=0);
}

int main(){
    int choice;
    char expr[200];
    do{
        printf("\n===== MODULE B: DATA STRUCTURES LAB =====\n");
        printf("1. Stack\n2. Queue (Linear + Circular)\n3. Singly Linked List\n4. Doubly Linked List\n5. Bracket Checker (Stack Application)\n0. Exit\nChoice: ");
        scanf("%d",&choice);
        if(choice==1) stack_menu();
        else if(choice==2) queue_menu();
        else if(choice==3) sll_menu();
        else if(choice==4) dll_menu();
        else if(choice==5){ printf("Enter expression e.g. {([ ])} : "); scanf("%s",expr); is_balanced(expr); }
    }while(choice!=0);
    printf("Goodbye!\n");
    return 0;
}
