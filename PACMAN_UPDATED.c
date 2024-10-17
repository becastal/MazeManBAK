#include "pacman.h"
#include <windows.h>

void printa_labirinto(labirinto L) {
	for (int i = 0; i < L.linhas; i++) {
		for (int j = 0; j < L.colunas; j++) {
			printf("%c", L.celulas[i][j]);
		}
		printf("\n");
	}
}

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

// Criado unicamente para representar o pacman no terminal com base nas direcoes que ele esta se movendo
char pacman_direction(Pacman pac, char direcao){
    if (direcao == 0) {
        return '^';
    }
    else if (direcao == 1){
        return '^';
    }
    else if (direcao == 1){
        return '>';
    }
    else if (direcao == 1){
        return '<';
    }
}

// Loop do jogo do pacman 
void pacman_game_loop("""ponteiro do Labirinto"""){
    Pacman pacman;
    Sleep(60);

    // printa_labirinto()
}