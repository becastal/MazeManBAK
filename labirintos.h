#ifndef labirintos_h
#define labirintos_h
#include <stdio.h>

typedef struct {
	int linhas, colunas;
	char** celulas;
} labirinto;

void gera_labirinto();
int posicao_valida(labirinto* L, int i, int j);
int posicao_aleatoria(labirinto* L, int tipo);
void printa_labirinto(labirinto L);
void algoritmo_binary_tree(labirinto* L);
void algoritmo_sidewinder(labirinto* L);
void algoritmo_aldous_border(labirinto* L);
void algoritmo_wilson(labirinto* L);
void dfs(int** distancia, labirinto* L, int linha, int coluna);
void resolve_dfs(labirinto* L);

#endif
