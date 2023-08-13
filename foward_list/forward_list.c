#include <stdio.h>
#include <stdlib.h>

#include "forward_list.h"

Node *node_construct(data_type value, Node *next){
    Node *n = (Node *)malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

void node_destroy(Node *n){
    free(n);
}

ForwardList *forward_list_construct(){
    ForwardList *l = (ForwardList *)calloc(1, sizeof(ForwardList));

    l->head = NULL;
    l->last = NULL;
    l->size = 0;

    return l;
}

int forward_list_size(ForwardList *l){
    return l->size;
}

data_type forward_list_find(ForwardList *l, void *key, int (*cmp_fn)(data_type data, void *key)){
    Node *n = l->head;

    while (n != NULL)
    {
        if (!cmp_fn(n->value, key))
            return n->value;

        n = n->next;
    }

    return 0;
}

void forward_list_push_front(ForwardList *l, data_type data){
    l->head = node_construct(data, l->head);
    l->size++;

    if (l->size == 1)
        l->last = l->head;
}

void forward_list_push_back(ForwardList *l, data_type data){
    Node *new_node = node_construct(data, NULL);

    if (l->last == NULL)
        l->head = l->last = new_node;
    else
        l->last = l->last->next = new_node;

    l->size++;
}

void forward_list_print(ForwardList *l, void (*print_fn)(data_type)){
    Node *n = l->head;

    printf("[");

    while (n != NULL)
    {
        print_fn(n->value);
        n = n->next;

        if (n != NULL)
            printf(", ");
    }

    printf("]");
}

data_type forward_list_get(ForwardList *l, int i){
    if (i < 0 || i >= l->size)
        exit(printf("Error: index out of bounds."));

    Node *n = l->head;
    for (int j = 0; j < i; j++)
        n = n->next;
    
    return n->value;
}

data_type forward_list_pop_front(ForwardList *l){
    if (l->head == NULL)
        exit(printf("Error: list is empty."));
    
    Node *n = l->head;
    l->head = l->head->next;
    data_type data = n->value;
    node_destroy(n);
    l->size--;
    
    if (l->size <= 1)
        l->last = l->head;
    
    return data;
}

ForwardList *forward_list_reverse(ForwardList *l){
    ForwardList *l2 = forward_list_construct();
    Node* n = l->head;

    while (n != NULL){
        forward_list_push_front(l2, n->value);
        n = n->next;
    }

    return l2;
}

void forward_list_clear(ForwardList *l){
    int n_itens = l->size;

    for (int i = 0; i < n_itens; i++)
        forward_list_pop_front(l);
}

void forward_list_remove(ForwardList *l, data_type val){
    Node *n = l->head;
    Node *prev = NULL;
    Node *new_n = NULL;

    while (n != NULL)
    {
        if (n->value == val)
        {
            if (prev == NULL)
                l->head = new_n = n->next;
            else
                prev->next = new_n = n->next;

            node_destroy(n);
            n = new_n;
            l->size--;
        }
        else
        {
            prev = n;
            n = n->next;
        }
    }
}

void forward_list_unique(ForwardList *l){
    if(l->size > 1){
        forward_list_sort(l);
        Node* prev = l->head;
        Node* n = prev->next;

        while(n->next != NULL){
            if(prev->value == n->value){
                prev->next = n->next;

                Node* aux = n;
                n = n->next;
                node_destroy(aux);
            }
        }
    }
}

void forward_list_cat(ForwardList *l, ForwardList *m){
    Node *n = m->head;

    while (n != NULL){
        forward_list_push_front(l, n->value);
        n = n->next;
    }
}

void forward_list_sort(ForwardList *l){
    Node* current = l->head;
    Node* index = NULL;
    int temp;

    if(l->head){
        while(current != NULL){
            index = current->next;

            while(index != NULL){
                if(current->value > index->value){
                    temp = current->value;
                    current->value = index->value;
                    index->value = temp;
                }
                index = index->next;
            }
            current = current->next;
        }
    }

    /**int swapped, i; 
    Node *ptr1; 
    Node *lptr = NULL; 
  
    
    if (l->head == NULL) 
        return; 
  
    do{ 
        swapped = 0; 
        ptr1 = l->head; 
  
        while (ptr1->next != lptr){ 

            if (ptr1->value > ptr1->next->value){ 
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); */
}

void forward_list_destroy(ForwardList *l){
    Node *n = l->head;
    while (n != NULL)
    {
        Node *next = n->next;
        node_destroy(n);
        n = next;
    }

    free(l);
}