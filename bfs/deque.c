#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"

const int block_size = 5;

typedef struct Deque{
    void*** array;
    int start_idx;
    int start_block;
    int end_idx;
    int end_block;
    int blocks_allocd;
}Deque;

void **array_calloc(){
    void** array = (void **)calloc(block_size, sizeof(void *));
    
    return array;
}

Deque *deque_create(){
    Deque *d = (Deque *)calloc(1, sizeof(Deque));

    d->array = (void ***)calloc(block_size, sizeof(void **));
    
    int middle = block_size/2;

    d->array[middle] = array_calloc();

    d->start_idx = 0;
    d->start_block = middle;
    d->end_idx = 0;
    d->end_block = middle;
    d->blocks_allocd = block_size;

    return d;
}

void deque_realloc(Deque* d){
    void*** new;
    void*** prev = d->array;

    if(d->start_block == -1 && d->end_block == d->blocks_allocd){
        new = (void ***)calloc(d->blocks_allocd + (block_size - 1), sizeof(void **));
        int middle = block_size/2;

        for(int i = 0; i < d->blocks_allocd; i++){
            if(prev[i]){
                new[i + middle] = prev[i];
            }
        }

        d->array = new;
        d->start_block = middle;
        d->end_block = d->end_block + middle - 1;
        d->blocks_allocd = d->blocks_allocd + (block_size - 1);
        free(prev);
    }
    else if(d->start_block == -1 && d->end_block != d->blocks_allocd){
        if(d->end_block + 2 == d->blocks_allocd){
            new = (void ***)calloc(d->blocks_allocd + 1, sizeof(void **));

            for(int i = 1; i <= d->blocks_allocd; i++){
                if(prev[i-1]){
                    new[i] = prev[i-1];
                }
            }

            d->start_idx = d->start_idx + block_size;
            d->end_idx = d->end_idx + block_size;
            d->start_block++;
            d->end_block++;

            d->blocks_allocd++;
            d->array = new;
            free(prev);
        }
        else{
            if(d->start_block == d->end_block && d->start_idx == d->end_idx){
                d->start_block = d->end_block = d->blocks_allocd/2;
                d->start_idx = d->end_idx = 0;
            }
            else{
                int deque_size = d->end_block - d->start_block + 1;
                int upper_space = d->blocks_allocd / deque_size;

                d->start_block = upper_space;
                d->end_block = upper_space + deque_size - 1;

                new = (void ***)calloc(d->blocks_allocd, sizeof(void **));
                for(int i = 0; i < deque_size; i++){
                    if(prev[i]){
                        new[i+upper_space] = prev[i];
                    }
                }

                d->array = new;
                free(prev);
            }
        }
    }
    else if(d->end_block == d->blocks_allocd && d->start_block != -1){
        if(d->start_block == 1){
            new = (void ***)calloc(d->blocks_allocd + 1, sizeof(void **));

            for(int i = 0; i < d->blocks_allocd; i++){
                if(prev[i]){
                    new[i] =  prev[i];
                }
            }

            d->blocks_allocd++;
            d->array = new;
            free(prev);
        }
        else{
            if(d->start_block == d->end_block && d->start_idx == d->end_idx){
                d->start_block = d->end_block = d->blocks_allocd/2;
                d->start_idx = d->end_idx = 0;
            }
            else{
                int deque_size = d->end_block - d->start_block + 1;
                int bottom_space = d->blocks_allocd / deque_size;

                new = (void ***)calloc(d->blocks_allocd, sizeof(void **));
                for(int i = d->end_block - 1; i > d->end_block - deque_size; i--){
                    if(prev[i]){
                        new[i- bottom_space] = prev[i];
                    }
                }

                d->end_block = d->blocks_allocd - bottom_space;
                d->start_block = d->end_block - deque_size + 1;

                d->array = new;
                free(prev);
            }
        }
    }
}

void deque_push_back(Deque *d, void *val){
    if(d->end_block == d->blocks_allocd){
        deque_realloc(d);
    }

    if(d->end_block == d->start_block && d->end_idx == d->start_idx){
        d->start_block = d->end_block = d->blocks_allocd/2;
        d->start_idx = d->end_idx = 0;
    }

    if(!d->array[d->end_block]){
        d->array[d->end_block] = array_calloc();
    }

    int item_idx = d->end_idx % block_size;
    d->array[d->end_block][item_idx] = val;

    d->end_idx = (d->end_idx + 1) % block_size;
    
    if(d->end_idx == 0){
        d->end_block++;
    }
}

void deque_push_front(Deque *d, void *val){
    d->start_idx--;

    if(d->start_idx == -1){
        d->start_block--;
        
        if(d->start_block == -1){
            deque_realloc(d);
        }
        
        d->start_idx = block_size - 1;
    }

    if(d->end_block == d->start_block && d->end_idx == d->start_idx){
        d->start_block = d->end_block = d->blocks_allocd/2;
        d->start_idx = d->end_idx = 0;
    }

    if(!d->array[d->start_block]){
        d->array[d->start_block] = array_calloc();
    }

    d->array[d->start_block][d->start_idx] = val;
}

void *deque_pop_back(Deque *d){
    void* val =  0;
    d->end_idx--;
    
    if(d->end_idx == -1){
        d->end_idx = block_size - 1;
        d->end_block--;

        val = d->array[d->end_block][d->end_idx];
        d->array[d->end_block][d->end_idx] = NULL;
    }
    else if(d->end_idx == 0){
        val = d->array[d->end_block][d->end_idx];
        d->array[d->end_block][d->end_idx] = NULL;
        free(d->array[d->end_block]);
        d->array[d->end_block] = NULL;
    }
    else{
        val = d->array[d->end_block][d->end_idx];
        d->array[d->end_block][d->end_idx] = NULL;
    }

    return val;
}

void *deque_pop_front(Deque *d){
    void* val = d->array[d->start_block][d->start_idx];
    d->array[d->start_block][d->start_idx] = NULL;

    d->start_idx++;

    if(d->start_idx == block_size){
        free(d->array[d->start_block]);
        d->array[d->start_block] = NULL;
        d->start_idx = 0;
        d->start_block++;
    }

    return val;
}

void array_destroy(void*** array, int tamanho){
    for(int i = 0; i < tamanho; i++){
        if(array[i]){
            for(int j = 0; j < block_size; j++){
                if(array[i][j]){
                    free(array[i][j]);
                }
            }
            free(array[i]);
        }
    }

    free(array);
}

void deque_destroy(Deque *d){
        if (d == NULL) {
        return;
    }

    array_destroy(d->array, d->blocks_allocd);
    free(d);
}

int empty_deque(Deque *d){
    if(d->start_block == d->end_block && d->start_idx == d->end_idx)
        return 1;
    else
        return 0;
}