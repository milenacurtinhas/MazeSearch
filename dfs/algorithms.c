
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

ResultData _default_result()
{
    ResultData result;

    result.caminho = NULL;
    result.custo_caminho = 0;
    result.nos_expandidos = 1;
    result.tamanho_caminho = 0;
    result.sucesso = 0;

    return result;
}

void expand_maze(Celula* c, Labirinto* l, Deque* d){
    deque_push_back(d, c);
    labirinto_atribuir(l, c->y, c->x, EXPANDIDO);
}

int valid_position(int x, int y, Labirinto* l){
    if(!is_out_of_maze(x, y, l)){
        if(labirinto_obter(l, y, x) != OCUPADO && labirinto_obter(l, y, x) != EXPANDIDO && labirinto_obter(l, y, x) != INICIO){
            return 1;
        }
    }

    return 0;
}

ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();

    Celula *i = malloc(sizeof(Celula));
    i->x = inicio.x;
    i->y = inicio.y;
    i->prev = NULL;
    i->dist_from_prev = 0;

    Celula *atual = i;

    labirinto_atribuir(l, atual->y, atual->x, INICIO);
    labirinto_atribuir(l, fim.y, fim.x, FIM);

    Deque *popped_deque = deque_create();
    deque_push_back(popped_deque, atual);

    Deque *expanded_deque = deque_create();

    while((atual->x != fim.x) || (atual->y != fim.y)){
        if(valid_position(atual->x, atual->y - 1, l)){
            Celula *acima = malloc(sizeof(Celula));
            acima->x = atual->x;
            acima->y = atual->y - 1;
            acima->prev = atual;
            acima->dist_from_prev = 1;
            expand_maze(acima, l, expanded_deque);
        }
        
        if(valid_position(atual->x + 1, atual->y - 1, l)){
            Celula *diagonal_dir_sup = malloc(sizeof(Celula));
            diagonal_dir_sup->x = atual->x + 1;
            diagonal_dir_sup->y = atual->y - 1;
            diagonal_dir_sup->prev = atual;
            diagonal_dir_sup->dist_from_prev = sqrt(2);
            expand_maze(diagonal_dir_sup, l, expanded_deque);
        }
        
        if(valid_position(atual->x + 1, atual->y, l)){
            Celula *lateral_dir = malloc(sizeof(Celula));
            lateral_dir->x = atual->x + 1;
            lateral_dir->y = atual->y;
            lateral_dir->prev = atual;
            lateral_dir->dist_from_prev = 1;
            expand_maze(lateral_dir, l, expanded_deque);
        }
        
        if(valid_position(atual->x + 1, atual->y + 1, l)){
            Celula *diagonal_dir_inf = malloc(sizeof(Celula));
            diagonal_dir_inf->x = atual->x + 1;
            diagonal_dir_inf->y = atual->y + 1;
            diagonal_dir_inf->prev = atual;
            diagonal_dir_inf->dist_from_prev = sqrt(2);
            expand_maze(diagonal_dir_inf, l, expanded_deque);
        }
        
        if(valid_position(atual->x, atual->y + 1, l)){
            Celula *abaixo = malloc(sizeof(Celula));
            abaixo->x = atual->x;
            abaixo->y = atual->y + 1;
            abaixo->prev = atual;
            abaixo->dist_from_prev = 1;
            expand_maze(abaixo, l, expanded_deque);
        }
        
        if(valid_position(atual->x - 1, atual->y + 1, l)){
            Celula *diagonal_esq_inf = malloc(sizeof(Celula));
            diagonal_esq_inf->x = atual->x - 1;
            diagonal_esq_inf->y = atual->y + 1;
            diagonal_esq_inf->prev = atual;
            diagonal_esq_inf->dist_from_prev = sqrt(2);
            expand_maze(diagonal_esq_inf, l, expanded_deque);
        }
        
        if(valid_position(atual->x - 1, atual->y, l)){
            Celula *lateral_esq = malloc(sizeof(Celula));
            lateral_esq->x = atual->x - 1;
            lateral_esq->y = atual->y;
            lateral_esq->prev = atual;
            lateral_esq->dist_from_prev = 1;
            expand_maze(lateral_esq, l, expanded_deque);
        }
        
        if(valid_position(atual->x - 1, atual->y - 1, l)){
            Celula *diagonal_esq_sup = malloc(sizeof(Celula));
            diagonal_esq_sup->x = atual->x - 1;
            diagonal_esq_sup->y = atual->y - 1;
            diagonal_esq_sup->prev = atual;
            diagonal_esq_sup->dist_from_prev = sqrt(2);
            expand_maze(diagonal_esq_sup, l, expanded_deque);
        }

        if(empty_deque(expanded_deque)){
            break;
        }
        atual = deque_pop_back(expanded_deque);
        result.nos_expandidos++;
        deque_push_back(popped_deque, atual);
    }

    if((atual->x != fim.x) || (atual->y != fim.y)){
        result.sucesso = 0;
    }
    else{
        result.caminho = (Celula *)malloc(10 * sizeof(Celula));
        int n = 0, malloc_size = 10;
        Celula *caminho = atual;
        while(caminho != NULL){
            result.caminho[n].x = caminho->y;
            result.caminho[n].y = caminho->x;
            n++;

            result.custo_caminho = result.custo_caminho + caminho->dist_from_prev;
            result.tamanho_caminho++;
            caminho = caminho->prev;

            if(n == malloc_size){
                result.caminho = realloc(result.caminho, (malloc_size * 2) * sizeof(Celula));
                malloc_size = malloc_size * 2;
            }       
        }
        result.sucesso = 1;
    }

    deque_destroy(expanded_deque);
    deque_destroy(popped_deque);
    
    return result;
}