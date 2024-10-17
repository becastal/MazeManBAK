#include <labirintos.h>
#include <pacman.h>

// Altera a posicao do pacman na matriz
void pacman_move(Pacman *pac, int direcao){
    int di_2[] = {-2, 2, 0, 0};
    int dj_2[] = {0, 0, 2, -2};
    int di_1[] = {-1, 1, 0, 0};
    int dj_1[] = {0, 0, 1, -1};
    pac->posicao_linha += di_1[direcao];
    pac->posicao_linha += dj_1[direcao];
    pac->direcao_olhando = direcao;

}

