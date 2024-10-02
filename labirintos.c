#include "labirintos.h"
#include <stdlib.h>
#include <time.h>

// TODO: talvez visualizar a construcao de cada algoritmo? seria mt maneiro.

void gera_labirinto() {
	srand(time(NULL));

	puts("[+] novo labirinto!");
	labirinto novo_labirinto;

	int ok_linhas = 0;	
	while (!ok_linhas) {
		printf("[?] indique a quantidade de linhas: ");
		scanf("%d", &novo_labirinto.linhas);

		ok_linhas = (novo_labirinto.linhas >= 1);
	}
	novo_labirinto.linhas = 2 * novo_labirinto.linhas + 1;

	int ok_colunas = 0;	
	while (!ok_colunas) {
		printf("[?] indique a quantidade de colunas: ");
		scanf("%d", &novo_labirinto.colunas);

		ok_colunas = (novo_labirinto.colunas >= 1);
	}
	novo_labirinto.colunas = 2 * novo_labirinto.colunas + 1;

	novo_labirinto.celulas = (char**) malloc(novo_labirinto.linhas * sizeof(char*));
	for (int i = 0; i < novo_labirinto.linhas; i++) {
		novo_labirinto.celulas[i] = (char*) malloc(novo_labirinto.colunas * sizeof(char));
	}

	for (int i = 0; i < novo_labirinto.linhas; i++) {
		for (int j = 0; j < novo_labirinto.colunas; j++) {
			novo_labirinto.celulas[i][j] = (i & 1 && j & 1 ? ' ' : '#');	
		}
	}

	puts("[!] possiveis algoritmos para gerar o labirinto");
	puts("[0] algoritmo da binary tree.");
	puts("[1] algoritmo de sinewinder.");
	puts("[2] algoritmo de aldous-border.");
	puts("[3] algoritmo de wilson.");
	puts("[4] algoritmo de hunt-and-kill.");
	puts("[5] algoritmo de recursive backtracking.");

	int ok_algoritmo = 0;
	while (!ok_algoritmo) {
		printf("[?] selecione uma das opcoes: ");
		int selecao;
		scanf("%d", &selecao);

		ok_algoritmo = 1;
		switch (selecao) {
			case 1:
				algoritmo_binary_tree(&novo_labirinto);
				break;
			default:
				puts("[e] selecao invalida!");
				ok_algoritmo = 0;
		}
	}

	printa_labirinto(novo_labirinto);
	printf("[?] salvar este labirinto em arquivo (s/n)? ");

	for (int i = 0; i < novo_labirinto.linhas; i++) {
		free(novo_labirinto.celulas[i]);
	}
	free(novo_labirinto.celulas);
}

void printa_labirinto(labirinto L) {
	for (int i = 0; i < L.linhas; i++) {
		for (int j = 0; j < L.colunas; j++) {
			printf("%c", L.celulas[i][j]);
		}
		printf("\n");
	}
}

void algoritmo_binary_tree(labirinto* L) {
	for (int posicao_linha = 1; posicao_linha < L->linhas; posicao_linha += 2) {
		for (int posicao_coluna = 1; posicao_coluna < L->colunas; posicao_coluna += 2) {
			int libera_direita = rand() & 1;
			if (posicao_linha == L->linhas - 2) libera_direita = 1;
			if (posicao_coluna == L->colunas - 2) libera_direita = 0;
			if (posicao_linha == L->linhas - 2 && posicao_coluna == L->colunas - 2) break;

			if (libera_direita) {
				L->celulas[posicao_linha][posicao_coluna + 1] = ' ';
			} else {
				L->celulas[posicao_linha + 1][posicao_coluna] = ' ';
			}
		}
	}
}
