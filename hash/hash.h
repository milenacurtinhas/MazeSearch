#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

typedef struct HashTable HashTable;

typedef int (*HashFunction)(HashTable *, void *);
typedef int (*CmpFunction)(void *k1, void *k2);

typedef struct HashTableItem HashTableItem;

struct HashTableItem{
    void *key;
    void *val;
};

typedef struct HashTableIterator HashTableIterator;

/**
 * @brief Cria a tabela hash
 * @param table_size
 * Número de buckets na tabela hash
 * @param hash_fn
 * Ponteiro de função que recebe como entrada uma chave do tipo void* e retorna um inteiro
 * @param cmp_fn
 * Ponteiro de função que recebe como entrada dois void* representando duas chaves e tem a mesma semântica da função strcmp
 * @return HashTable*
 * Retorna o ponteiro da tabela hash recém alocada.
 */
HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn);

// funcao para insercao/atualizacao de pares chave-valor em O(1).
// Se a chave ja existir, atualiza o valor e retorna o valor antigo para permitir desalocacao.
void *hash_table_set(HashTable *h, void *key, void *val);

// retorna o valor associado com a chave key ou NULL se ela nao existir em O(1).
void *hash_table_get(HashTable *h, void *key);

// libera o espaco alocado para a tabela hash
void hash_table_destroy(HashTable *h);

// valor do item
void* get_value(HashTableItem* item);

// chave do item
void* get_key(HashTableItem* item);

// numero de buckets
int hash_table_size(HashTable *h);

// numero de elementos inseridos
int hash_table_num_elems(HashTable *h);


// remove o par chave-valor e retorna o valor ou NULL se nao existir tal chave em O(1).
//void *hash_table_pop(HashTable *h, void *key);

// cria um novo iterador para a tabela hash
HashTableIterator *hash_table_iterator(HashTable *h);

// retorna 1 se o iterador chegou ao fim da tabela hash ou 0 caso contrario
int hash_table_iterator_is_over(HashTableIterator *it);

// retorna o proximo par chave valor da tabela hash
HashTableItem *hash_table_iterator_next(HashTableIterator *it);

//ForwardList** _get_buckets(HashTable* h);

// desaloca o iterador da tabela hash
void hash_table_iterator_destroy(HashTableIterator *it);

#endif