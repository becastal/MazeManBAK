#include "labirintos.h"

typedef struct {
	int direcao_olhando;
	int posicao_linha;
	int posicao_coluna;
	labirinto mapa;
} Pacman;

typedef struct {
	int direcao_olhando;
	int posicao_linha;
	int posicao_coluna;
	labirinto mapa;
} Fanstasma;

// Altera a posicao do pacman na matriz
void pacman_move(Pacman *pac, int direcao){

    pac->posicao_linha += di_1[direcao];
    pac->posicao_linha += dj_1[direcao];
    pac->direcao_olhando = direcao;

}

// Criado unicamente para representar o pacman no terminal com base nas direcoes que ele esta se movendo
char pacman_direction(Pacman pac, char direcao){
    if (direcao == 0) {
        return '^';
    }
    else if (direcai == 1){
        return '^';
    }
    else if (direcai == 1){
        return '>';
    }
    else if (direcai == 1){
        return '<';
    }
}


