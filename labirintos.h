#ifndef labirintos_h
#define labirintos_h
#include <stdio.h>

typedef struct labirinto labirinto;
struct labirinto {
	int linhas, colunas;
	char** celulas;
};

void gera_labirinto();
int posicao_valida(labirinto* L, int i, int j);
void printa_labirinto(labirinto L);
void algoritmo_binary_tree(labirinto* L);
void algoritmo_sidewinder(labirinto* L);
void algoritmo_aldous_border(labirinto* L);

#endif
