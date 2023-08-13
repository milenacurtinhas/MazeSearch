#ifndef _DEQUE_H_
#define _DEQUE_H_

typedef struct Deque Deque;

// aloca memoria para o deque incializando com nulo
void **array_calloc();

// criacao do deque
Deque *deque_create();

// realoca espaço
void deque_realloc(Deque* d);

// funcoes para insercao na direita e esquerda (devem ser feitas em O(1), com eventual realloc)
void deque_push_back(Deque *d, void *val);
void deque_push_front(Deque *d, void *val);

// funcoes para remocao na direita e esquerda (devem ser feitas em O(1))
void *deque_pop_back(Deque *d);
void *deque_pop_front(Deque *d);

// libera o espaco alocado para o array
void array_destroy(void*** array, int tamanho);

// libera o espaco alocado para o deque
void deque_destroy(Deque *d);

#endif