#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

typedef struct{
    void *data;
    double priority;
} HeapNode;

struct Heap{
    HeapNode *node;
    HashTable *h;
    int size;
    int capacity;
};

Heap *heap_construct(HashTable *h){
    Heap* heap = calloc (1, sizeof(Heap));

    heap->node = calloc (HEAP_SIZE, sizeof(HeapNode));
    heap->h = h;
    heap->size = 0;
    heap->capacity = HEAP_SIZE;

    return heap;
}

void *heap_push(Heap *heap, void *data, int priority){
    //caso não tenha mais espaço no heap faz o realloc
    if(heap->size == heap->capacity){
        heap->node = realloc (heap->node, (heap->capacity +  HEAP_SIZE) * sizeof(Heap));
        heap->capacity = heap->capacity + HEAP_SIZE;
    }

    int *aux = NULL;
    
    //checa se o heap está vazio
    //caso não esteja, busca um indice no heap que tenha a mesma célula
    if(!heap_empty(heap)){
        aux = (int*)hash_table_get(heap->h, data);
    }

    //se for encontrada uma posição no heap
    //verifica se a prioridade previa é menor que a nova
    //caso seja, a nova celula é retornada
    //caso não seja, a nova celula substitui a antiga, o heap é reordenado e a prévia celula é retornada
    if(aux){
        int old_priority = heap->node[*aux].priority;
        
        if(old_priority < priority){
            return data;
        }
        else{
            heap->node[*aux].priority = priority;

            for(int i = heap->size / 2 -1; i >= 0; i--){
                heap = heap_min(heap, i);
            }
            
            return data;
        }
    }

    //se não for encontrada a nova célula é adicionada no heap e ele é reordenado
    else{
        heap->node[heap->size].data = data;
        heap->node[heap->size].priority = priority;
        
        int *pos = malloc(sizeof(int));
        *(pos) = heap->size;
        hash_table_set(heap->h, data, pos);
        heap->size++;

        for(int i = heap->size / 2 - 1; i >= 0; i--){
            heap = heap_min(heap, i);
        }

        return NULL;
    }
}

bool heap_empty(Heap *heap){
    if(heap->size == 0)
        return 1;

    else
        return 0;
}

Heap *heap_min(Heap *heap, int i){
    if (heap->size <= 1){

        return heap;
    }
    else{
        int smallest = i, l = 2 * i + 1, r = 2 * i + 2;

        if (l < heap->size && (heap->node[l].priority < heap->node[smallest].priority))
            smallest = l;

        if (r < heap->size && (heap->node[r].priority < heap->node[smallest].priority))
            smallest = r;

        if (smallest != i){
            HeapNode temp = heap->node[i];
            heap->node[i] = heap->node[smallest];
            heap->node[smallest] = temp;

            int *pos_1 = malloc(sizeof(int));
            *(pos_1) = i;
            void* old_val_1 = hash_table_set(heap->h, heap->node[i].data, pos_1);
            free(old_val_1);

            int *pos_2 = malloc(sizeof(int));
            *(pos_2) = smallest;
            void* old_val_2 = hash_table_set(heap->h, heap->node[smallest].data, pos_2);
            free(old_val_2);

            heap = heap_min(heap, smallest);
        }

        return heap;
    }
}

double heap_min_priority(Heap *heap){
    return heap->node[0].priority;
}

void *heap_pop(Heap *heap){
    void* celula = heap->node[0].data;
    HashTableItem *it = hash_table_get(heap->h, celula);
    free(it);
    //free(it->val);
//    free(it->key);

    heap->size--;
    heap->node[0] = heap->node[heap->size];
    heap->node[heap->size].data = NULL;
    heap->node[heap->size].priority = 0;

    remove_from_hash(heap->h, celula);

    if(heap->size == 1){
        int *pos = malloc(sizeof(int));
        *(pos) = heap->size - 1;

        int* old_pos = hash_table_set(heap->h, heap->node[0].data, pos);    
        free(old_pos);    
    }

    for(int i = heap->size / 2 - 1; i >= 0; i--){
        heap = heap_min(heap, i);
    }

    return celula;
}

void heap_destroy(Heap *heap){
    free(heap->node);
    free(heap);
}