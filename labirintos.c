#include "labirintos.h"
#include <stdlib.h>
#include <time.h>

// TODO: talvez visualizar a construcao de cada algoritmo? seria mt maneiro.
// TODO: talvez visualizar o desenvolvimento do caminho.
// TODO: ver como srand funciona. se pa que vale fazer srand antes de todo algoritmo.
// TODO: 

int dx2[] = {-2, 2, 0, 0};
int dy2[] = {0, 0, -2, 2};
int dx1[] = {-1, 1, 0, 0};
int dy1[] = {0, 0, -1, 1};

void gera_labirinto() {
	srand(time(NULL));

	puts("[+] novo labirinto!");
	labirinto novo_labirinto;

	int ok_linhas = 0;	while (!ok_linhas) {
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
	//puts("[3] algoritmo de wilson.");
	//puts("[4] algoritmo de hunt-and-kill.");
	//puts("[5] algoritmo de recursive backtracking.");

	int ok_algoritmo = 0;
	while (!ok_algoritmo) {
		printf("[?] selecione uma das opcoes: ");
		int selecao;
		scanf("%d", &selecao);

		ok_algoritmo = 1;
		switch (selecao) {
			case 0:
				algoritmo_binary_tree(&novo_labirinto);
				break;
			case 1:
				algoritmo_sidewinder(&novo_labirinto);
				break;
			case 2:
				algoritmo_aldous_border(&novo_labirinto);
				break;
			case 3:
				algoritmo_wilson(&novo_labirinto);
				break;
			default:
				puts("[e] selecao invalida!");
				ok_algoritmo = 0;
		}
	}

	printa_labirinto(novo_labirinto);

	resolve_dfs(&novo_labirinto);

	for (int i = 0; i < novo_labirinto.linhas; i++) {
		free(novo_labirinto.celulas[i]);
	}
	free(novo_labirinto.celulas);
}

int posicao_valida(labirinto* L, int i, int j) {
    return i >= 1 && j >= 1 && i < L->linhas - 1 && j < L->colunas - 1;
}

int posicao_aleatoria(labirinto* L, int tipo) {
    int min = 1, max = (tipo ? L->linhas - 2 : L->colunas - 2);
    return min + 2 * (rand() % ((max - min) / 2 + 1));
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
	// pra cada posicao da matriz, tira ou a parede da direita ou a parede de baixo;
	// complexidade O(nm);

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

void algoritmo_sidewinder(labirinto* L) {
	// pra cada posicao da matriz, tira ou a parede de cima ou a parede da direita, mas quando existe uma mudanca
	// na sequencia (por exemplo, lado -> lado -> direita), ele nao corta a direita da posicao que eu to, mas sim
	// uma aleatoria do componente conexo anterior;
	// complexidade O(nm);
	
    for (int posicao_linha = 1; posicao_linha < L->linhas; posicao_linha += 2) {
        int inicio_run = 1;
        
        for (int posicao_coluna = 1; posicao_coluna < L->colunas; posicao_coluna += 2) {
            int libera_direita = rand() & 1;
            
            if (posicao_coluna == L->colunas - 2) libera_direita = 0;
            if (posicao_linha == 1) libera_direita = 1;
			if (posicao_linha == 1 && posicao_coluna == L->colunas - 2) break;

            if (libera_direita) {
                L->celulas[posicao_linha][posicao_coluna + 1] = ' ';
            } else {
                int posicao_norte = inicio_run + 2 * (rand() % ((posicao_coluna - inicio_run) / 2 + 1));
                L->celulas[posicao_linha - 1][posicao_norte] = ' ';
                inicio_run = posicao_coluna + 2;
            }
        }
    }
}

void algoritmo_aldous_border(labirinto* L) {
	// pra uma posicao aleatoria, seguir escolhendo movimentos aleatorios ate ter visitado todos os vertices.
	// se voce entrar em uma posicao nao antes visitada, voce liga as duas celulas cortando a parede, se nao,
	// voce so segue. bizarro tentar achar a complexidade disso. existe um mundo que esse algoritmo nunca acaba;
	// complexidade: O(sei_la * ruim);
	// na real a complexidade media fica ~O(nmlogn);

    int contador_visitados = 0;
    
    int** visitado = (int**) malloc(L->linhas * sizeof(int*));
    for (int i = 0; i < L->linhas; i++) {
        visitado[i] = (int*) malloc(L->colunas * sizeof(int));
        for (int j = 0; j < L->colunas; j++) {
            visitado[i][j] = 0;
            if (i % 2 == 1 && j % 2 == 1) {
                contador_visitados++;
            }
        }
    }

    int posicao_linha = posicao_aleatoria(L, 1);
    int posicao_coluna = posicao_aleatoria(L, 0);
    contador_visitados--;
    visitado[posicao_linha][posicao_coluna] = 1;

    while (contador_visitados > 0) {
        int movimento = rand() % 4;
        int nova_posicao_linha = posicao_linha + dx2[movimento];
        int nova_posicao_coluna = posicao_coluna + dy2[movimento];

        if (!posicao_valida(L, nova_posicao_linha, nova_posicao_coluna)) {
            continue;
        }

        if (!visitado[nova_posicao_linha][nova_posicao_coluna]) {
            L->celulas[(posicao_linha + nova_posicao_linha) / 2][(posicao_coluna + nova_posicao_coluna) / 2] = ' ';
            visitado[nova_posicao_linha][nova_posicao_coluna] = 1;
            contador_visitados--;
        }

        posicao_linha = nova_posicao_linha;
        posicao_coluna = nova_posicao_coluna;
    }

    for (int i = 0; i < L->linhas; i++) {
        free(visitado[i]);
    }
    free(visitado);
}

void algoritmo_wilson(labirinto* L) {

}

void dfs(int** distancia, labirinto* L, int linha, int coluna) {
	for (int i = 0; i < 4; i++) {
		int nova_linha = linha + dx1[i], nova_coluna = coluna + dy1[i];
		if (posicao_valida(L, nova_linha, nova_coluna) && 
			distancia[nova_linha][nova_coluna] == -1   &&
			L->celulas[nova_linha][nova_coluna] != '#') {
			distancia[nova_linha][nova_coluna] = distancia[linha][coluna] + 1;
			dfs(distancia, L, nova_linha, nova_coluna);
		}
	}
}

void resolve_dfs(labirinto* L) {
	int** distancia = (int**) malloc(L->linhas * sizeof(int*));
	for (int i = 0; i < L->linhas; i++) {
		distancia[i] = (int*) malloc(L->colunas * sizeof(int));
		for (int j = 0; j < L->colunas; j++) {
			distancia[i][j] = -1;
		}
	}

	distancia[1][1] = 0;
	dfs(distancia, L, 1, 1);

	char** celulas_resolvido = L->celulas;
	int posicao_linha = L->linhas - 2, posicao_coluna = L->colunas - 2;
	while (distancia[posicao_linha][posicao_coluna] >= 0) {
		celulas_resolvido[posicao_linha][posicao_coluna] = '*';
		
		for (int i = 0; i < 4; i++) {
			int nova_posicao_linha = posicao_linha + dx1[i], nova_posicao_coluna = posicao_coluna + dy1[i];
			if (distancia[nova_posicao_linha][nova_posicao_coluna] + 1 == distancia[posicao_linha][posicao_coluna]) {

				posicao_linha = nova_posicao_linha;
				posicao_coluna = nova_posicao_coluna;
				break;
			}
		}
	}

	for (int i = 0; i < L-> linhas; i++) {
		for (int j = 0; j < L->colunas; j++) {
			printf("%s", (celulas_resolvido[i][j] == '*' ? "\033[1;31m" : "\033[0m"));
			printf("%c", celulas_resolvido[i][j]);
		}
		puts("");
	}

    for (int i = 0; i < L->linhas; i++) {
        free(distancia[i]);
    }
    free(distancia);
}
