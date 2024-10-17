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

void pacman_move(Pacman *pac, int direcao);
char pacman_direction(Pacman pac, char direcao);

