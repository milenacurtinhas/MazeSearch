
#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include "labirinto.h"

typedef struct{
    int x;
    int y;
    void *prev;
    double dist_from_prev;
} Celula;

typedef struct
{
    Celula *caminho;
    double custo_caminho;
    int tamanho_caminho;
    int nos_expandidos;
    int sucesso;
} ResultData;

void expand_maze(Celula* c, Labirinto* l, Deque* d);
int valid_position(int x, int y, Labirinto* l);
ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim);

#endif