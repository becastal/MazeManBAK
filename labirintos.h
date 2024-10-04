#ifndef labirintos_h
#define labirintos_h
#include <stdio.h>

typedef struct {
	int linhas, colunas;
	char** celulas;
} labirinto;

typedef struct {
	char direcao_olhando;
	int posicao_linha;
	int posicao_coluna;
	labirinto mapa;
} Pacman;

typedef struct {
	char direcao_olhando;
	int posicao_linha;
	int posicao_coluna;
	labirinto mapa;
} Fanstasma;

void gera_labirinto();
int posicao_valida(labirinto* L, int i, int j);
int posicao_aleatoria(labirinto* L, int tipo);
void printa_labirinto(labirinto L);
void algoritmo_binary_tree(labirinto* L);
void algoritmo_sidewinder(labirinto* L);
void algoritmo_aldous_border(labirinto* L);
void algoritmo_hunt_and_kill(labirinto* L);
void dfs(int** distancia, labirinto* L, int linha, int coluna);
void resolve_dfs(labirinto* L);

#endif
