#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "forward_list.h"

struct HashTable{
    ForwardList **buckets;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
    int table_size;
    int n_elements;
};

struct HashTableIterator{
    HashTable *hash_tbl;
    int bucket_idx;
    Node *node;
    int current_element;
};

HashTableItem* _hash_pair_construct(void *key, void *val){
    HashTableItem *p = calloc(1, sizeof(HashTableItem));
    p->key = key;
    p->val = val;
    return p;
}

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn){
    HashTable *hash_tbl = calloc(1, sizeof(HashTable));

    hash_tbl->table_size = table_size;
    hash_tbl->hash_fn = hash_fn;
    hash_tbl->cmp_fn = cmp_fn;
    hash_tbl->buckets = calloc(table_size, sizeof(ForwardList *));
    hash_tbl->n_elements = 0;

    return hash_tbl;
}

void* hash_table_set(HashTable *h, void *key, void *val){
    int key_val = h->hash_fn(h, key);
    

    if(!h->buckets[key_val]){
        HashTableItem* new_item = _hash_pair_construct(key, val);
        h->buckets[key_val] = forward_list_construct();
        forward_list_push_back(h->buckets[key_val], new_item);

        h->n_elements++;
    
        return NULL;
    }
    else{
        Node* aux = h->buckets[key_val]->head;

        while(aux){
            if(!h->cmp_fn(key, get_key(aux->value))){
                HashTableItem* old_item = aux->value;
                void* old_celula = old_item->val;
                old_item->val = val;

                return old_celula;
            }
            
            aux = aux->next;
        }

        HashTableItem* new_item = _hash_pair_construct(key, val);
        forward_list_push_back(h->buckets[key_val], new_item);

        h->n_elements++;

        return NULL;
    }
}

void *hash_table_get(HashTable *h, void *key){
    int key_val = h->hash_fn(h, key);
    
    if(h->buckets[key_val]){
        Node* aux = h->buckets[key_val]->head;

        while(aux){
            if(!h->cmp_fn(key, get_key(aux->value))){
                void* value = get_value(aux->value);
                return value;
            }
            aux = aux->next;
        }
    }
    
    return NULL;
}

void* get_value(HashTableItem* item){
    return item->val;
}

void* get_key(HashTableItem* item){
    return item->key;
}

int hash_table_size(HashTable *h){
    return h->table_size;
}

int hash_table_num_elems(HashTable *h){
    return h->n_elements;
}

void hash_table_destroy(HashTable *h){

    for(int i = 0; i < h->table_size; i++){
        if(h->buckets[i] != NULL){
            forward_list_destroy(h->buckets[i]);
        }
    }

    free(h->buckets);
    free(h);
}

HashTableIterator *hash_table_iterator(HashTable *h){
    HashTableIterator *hash_it = calloc(1, sizeof(HashTableIterator));

    hash_it->hash_tbl = h;
    hash_it->bucket_idx = -1;
    hash_it->node = NULL;
    hash_it->current_element = 0;

    return hash_it;
}

int hash_table_iterator_is_over(HashTableIterator *it){
    int elem = hash_table_num_elems(it->hash_tbl);
    if(it->current_element >= elem){
        return 1;
    }
    else{
        return 0;
    }
}

HashTableItem *hash_table_iterator_next(HashTableIterator *it){
    if(it->node){
        it->node = it->node->next;

        if(it->node){
            it->current_element++;

            return it->node->value;
        }
    }

    it->bucket_idx++;
    
    ForwardList *fw_list = NULL;
    while(it->bucket_idx < hash_table_size(it->hash_tbl)){
        if((fw_list = it->hash_tbl->buckets[it->bucket_idx]) && fw_list->head){
            break;
        }
        else{
            it->bucket_idx++;
        }
    }
    
    if(it->bucket_idx < hash_table_size(it->hash_tbl)){
        it->node = fw_list->head;
        it->current_element++;
        return it->node->value;
    }
    else{
        return NULL;
    }
}

void hash_table_iterator_destroy(HashTableIterator *it){
    free(it);
}

void remove_from_hash(HashTable *h, void *key){
    int key_val = h->hash_fn(h, key);
    h->n_elements--;

    if(h->buckets[key_val]){
        Node* aux = h->buckets[key_val]->head;
        Node* prev = NULL;

        while(aux){
            if(!h->cmp_fn(key, get_key(aux->value))){
                h->buckets[key_val]->size--;

                if(h->buckets[key_val]->size == 0){
                    HashTableItem *item = aux->value;
                    free(item);
                    free(aux);
                    aux = NULL;

                    free(h->buckets[key_val]);
                    h->buckets[key_val] = NULL;
                }
                else{
                    if(prev){
                        prev->next = aux->next;
                        if(h->buckets[key_val]->last == aux){
                            h->buckets[key_val]->last = prev;
                        }
                    }
                    else{
                        h->buckets[key_val]->head = aux->next;
                    }
                    
                    HashTableItem *item = aux->value;
                    free(item);
                    free(aux);   
                }
                break;
            }
            prev = aux;
            aux = aux->next;
        }
    }    
}