#include "labirintos.h"
//#include "arquivos.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

// TODO: talvez visualizar a construcao de cada algoritmo? seria mt maneiro.
// TODO: talvez visualizar o desenvolvimento do caminho. mais maneiro que o de formar 
// TODO: ver como srand funciona. se pa que vale fazer srand antes de todo algoritmo.
// TODO: implementar stack e queue. o dfs recursivo da segfault quando n, m > 1e6, talvez seja a recursao.

int di_2[] = {-2, 2, 0, 0};
int dj_2[] = {0, 0, 2, -2};
int di_1[] = {-1, 1, 0, 0};
int dj_1[] = {0, 0, 1, -1};

void gera_labirinto() {
	FILE *arq;
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
	puts("[3] algoritmo de hunt-and-kill.");
	//puts("[4] algoritmo de recursive backtracking.");

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
				algoritmo_hunt_and_kill(&novo_labirinto);
				break;
			default:
				puts("[e] selecao invalida!");
				ok_algoritmo = 0;
		}
	}

	printa_labirinto(novo_labirinto);
	
	getchar();
//	printf("[?] salvar esse arquivo (s/n)? ");
//	char selecao;
//	scanf(" %c", &selecao);
//	
//	if (selecao == 's' || selecao == 'S') {
//		return;
//	}
//	else if(selecao == 'n' || selecao == 'N'){
//		return;
//	}
//	else{
//		printf("faz certo burro");
//		return;		
//	}

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
        int nova_posicao_linha = posicao_linha + di_2[movimento];
        int nova_posicao_coluna = posicao_coluna + dj_2[movimento];

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

void algoritmo_hunt_and_kill(labirinto* L) {
	// bem proximo do aldous border, mas ele evita entrar nas que ele ja viu,
	// buscando uma nao vista antes e recomecando a busca.
	// complexidade: O(nm);
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

	int direcoes_vistas = 0; 
    while (contador_visitados > 0) {
        int movimento = rand() % 4;
        int nova_posicao_linha = posicao_linha + di_2[movimento];
        int nova_posicao_coluna = posicao_coluna + dj_2[movimento];

		int hunt_ok = 0;
		if (direcoes_vistas == 15) { 
			// todos os bits de direcoes vistas sao 1. ou seja, vi todos os lados e
			// nao consegui sair. 15 == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3).

			for (int i = 0; i < L->linhas && !hunt_ok; i += 2) {
				for (int j = 0; j < L->colunas && !hunt_ok; j += 2) {
					if (!visitado[i][j]) {
						posicao_linha = i;
						posicao_coluna = j;
						hunt_ok = 1;
					}
				}
			}
		}

		if (hunt_ok) {
			visitado[posicao_linha][posicao_coluna] = 1;
			contador_visitados--;
			hunt_ok = 0;
			direcoes_vistas = 0;
			continue;
		}

        if (!posicao_valida(L, nova_posicao_linha, nova_posicao_coluna) ||
			visitado[nova_posicao_linha][nova_posicao_coluna]) {
			direcoes_vistas |= (1 << movimento); // setando o bit do movimento que vi do movimento que vi
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

void dfs(int** distancia, labirinto* L, int linha, int coluna) {
	for (int i = 0; i < 4; i++) {
		int nova_linha = linha + di_1[i], nova_coluna = coluna + dj_1[i];
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
			int nova_posicao_linha = posicao_linha + di_1[i], nova_posicao_coluna = posicao_coluna + dj_1[i];
			if (distancia[nova_posicao_linha][nova_posicao_coluna] + 1 == distancia[posicao_linha][posicao_coluna]) {
				posicao_linha = nova_posicao_linha;
				posicao_coluna = nova_posicao_coluna;
				break;
			}
		}
	}

	getchar();
	int agora = 0, sair = 0;
	while (!sair) {
		printf("\033[H\033[J");
		for (int i = 0; i < L->linhas; i++) {
			for (int j = 0; j < L->colunas; j++) {
				printf("%s", (celulas_resolvido[i][j] == '*' ? "\033[1;31m" : "\033[0m"));
				if (celulas_resolvido[i][j] == '*' && distancia[i][j] > agora) {
					printf(" ");
					continue;
				}
				printf("%c", celulas_resolvido[i][j]);
			}
			puts("");
		}

		printf("...");

		char c = getchar();
		if (c == '\033') {
			c = getchar();
			if (c == '[') {
				c = getchar();
				if (c == 'C') agora++;
				else if (c == 'D') agora--;
			}
		}

		if (c == '\n') sair = 1;
	}
    for (int i = 0; i < L->linhas; i++) {
        free(distancia[i]);
    }
    free(distancia);
}
