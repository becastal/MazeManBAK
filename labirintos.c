#include "labirintos.h"
#include "estruturas.h"
//#include "arquivos.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
// TODO: talvez visualizar a construcao de cada algoritmo? seria mt maneiro.
// TODO: talvez visualizar o desenvolvimento do caminho. mais maneiro que o de formar 
// TODO: implementar stack e queue. o dfs recursivo da segfault quando n, m > 1e6, talvez seja a recursao.

int di_2[] = {-2, 2, 0, 0};
int dj_2[] = {0, 0, 2, -2};
int di_1[] = {-1, 1, 0, 0};
int dj_1[] = {0, 0, 1, -1};

void gera_labirinto() {
	puts("[+] novo labirinto!");
	labirinto novo_labirinto;

	// recebendo a quantidade de linhas, que deve ser um inteiro >= 1
	int ok_linhas = 0;
	while (!ok_linhas) {
		printf("[?] indique a quantidade de linhas: ");
		scanf("%d", &novo_labirinto.linhas);

		ok_linhas = (novo_labirinto.linhas >= 1);
		if (!ok_linhas) puts("[e] informe um valor inteiro >= 1!");
	}

	// recebendo a quantidade de colunas, que deve ser um inteiro >= 1
	int ok_colunas = 0;
	while (!ok_colunas) {
		printf("[?] indique a quantidade de colunas: ");
		scanf("%d", &novo_labirinto.colunas);

		ok_colunas = (novo_labirinto.colunas >= 1);
		if (!ok_colunas) puts("[e] informe um valor inteiro >= 1!");
	}

	// construindo de fato o labirinto inicial, um pouco de matematica faz o labirinto inicialmente ter
	// o formato: ####### com a quantidade de celunas nas linhas e nas colunas conforme especificado.
	//            # # # #
	//            #######
	//            # # # #
	//            ####### ...
	
	novo_labirinto.linhas = 2 * novo_labirinto.linhas + 1;
	novo_labirinto.colunas = 2 * novo_labirinto.colunas + 1;
	novo_labirinto.celulas = (char**) malloc(novo_labirinto.linhas * sizeof(char*));
	for (int i = 0; i < novo_labirinto.linhas; i++) {
		novo_labirinto.celulas[i] = (char*) malloc(novo_labirinto.colunas * sizeof(char));
	}

	for (int i = 0; i < novo_labirinto.linhas; i++) {
		for (int j = 0; j < novo_labirinto.colunas; j++) {
			novo_labirinto.celulas[i][j] = (i & 1 && j & 1 ? ' ' : '#');	
			// pro formato de xadrez apresentado, as coordenadas das linhas e das colunas devem ser ambas
			// impares. x & 1 eh a mesma coisa que x % 2 == 1 mas mais maneiro.
		}
	}

	puts("[!] possiveis algoritmos para gerar o labirinto");
	puts("[0] algoritmo da binary tree.");
	puts("[1] algoritmo de sinewinder.");
	puts("[2] algoritmo de aldous-border.");
	puts("[3] algoritmo de hunt-and-kill.");
	puts("[4] algoritmo de backtracking.");

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
			case 4:
				algoritmo_backtracking(&novo_labirinto);
				break;
			default:
				puts("[e] selecao invalida!");
				ok_algoritmo = 0;
		}
	}
	
	char selecao_pacman;
	int ok_pacman = 0;

	while (!ok_pacman) {
		printf("[?] fazer esse labirinto ser decente para pacman (s/n)? ");
		scanf(" %c", &selecao_pacman);

		ok_pacman = (selecao_pacman == 's' || selecao_pacman == 'S' ||
					 selecao_pacman == 'n' || selecao_pacman == 'N');
		if (!ok_linhas) puts("[e] informe 's' ou 'n'.");
	}

	if (selecao_pacman == 's' || selecao_pacman == 'S') pacmaniza(&novo_labirinto);

	//printa_labirinto(novo_labirinto);
	resolve_dfs(&novo_labirinto);

//  FILE* arq;
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
	srand(time(NULL));

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
	srand(time(NULL));
	
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
	srand(time(NULL));

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
	srand(time(NULL));

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
			// todos os bits de direcoes vistas sao 1. ou seja, ja vi todos os lados e
			// nao consegui sair. esse numero porque 15 == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3).
			for (int i = 1; i < L->linhas - 1 && !hunt_ok; i += 2) {
				for (int j = 1; j < L->colunas - 1 && !hunt_ok; j += 2) {
					if (!visitado[i][j]) {
						for (int d = 0; d < 4; d++) {
							int ni = i + di_2[d];
							int nj = j + dj_2[d];
							if (posicao_valida(L, ni, nj) && visitado[ni][nj]) {
								posicao_linha = i;
								posicao_coluna = j;
								L->celulas[(i + ni) / 2][(j + nj) / 2] = ' ';
								hunt_ok = 1;
								break;
							}
						}
					}
				}
				if (hunt_ok) break;
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

void algoritmo_backtracking(labirinto* L) {
	// particularmente acho o mais elegante. bem parecido com dfs, mas cortando o caminho que comeco;
	// comeco no topo e sigo em direcoes aleatorias. se nao consigo mais sair, volto pra onde foi originado
	// essa posicao que estou agora (o inicio desse caminho sem volta);
	// complexidade: O(nm);
	srand(time(NULL));

    int** visitado = (int**) malloc(L->linhas * sizeof(int*));
    for (int i = 0; i < L->linhas; i++) {
        visitado[i] = (int*) malloc(L->colunas * sizeof(int));
        for (int j = 0; j < L->colunas; j++) {
            visitado[i][j] = 0;
        }
    }

	backtracking(visitado, L, 1, 1);

	for (int i = 0; i < L->linhas; i++) {
		free(visitado[i]);
	}
	free(visitado);
}

void backtracking(int** visitado, labirinto* L, int linha, int coluna) {
	visitado[linha][coluna] = 1;

	int direcao[] = {0, 1, 2, 3}; // randomizando a direcao que ele anda a cada passo
	for (int k = 0; k < 4; k++) {
		int r = rand() % 4;
		
		direcao[k] ^= direcao[r]; // jeito maneiro de trocar os valores no indice [r] e no indice[k]
		direcao[r] ^= direcao[k]; // com triplo xor. obrigado gabriel santana.
		direcao[k] ^= direcao[r];
	}

	for (int k = 0; k < 4; k++) {
		int movimento = direcao[k];
		int nova_linha = linha + di_2[movimento];
		int nova_coluna = coluna + dj_2[movimento];

		if (posicao_valida(L, nova_linha, nova_coluna) && 
			!visitado[nova_linha][nova_coluna]) {
			L->celulas[(linha + nova_linha) / 2][(coluna + nova_coluna) / 2] = ' '; 
			backtracking(visitado, L, nova_linha, nova_coluna);
		}
	}
}

void pacmaniza(labirinto* L) {
	srand(time(NULL));

	int ok_conectividade = 0, conexoes;
	while (!ok_conectividade) {
		printf("[?] indique a quantidade de novas conexoes: ");
		scanf("%d", &conexoes);

		ok_conectividade = (conexoes >= 1);
		if (!ok_conectividade) puts("[e] informe um valor inteiro >= 1.");
	}

	// gerando as novas conexoes;
	for (int feito = 0; feito < conexoes;) {
		int linha = posicao_aleatoria(L, 1);
		int coluna = posicao_aleatoria(L, 0);

		int direcao = rand() % 4;

		int nova_linha = linha + di_1[direcao];
		int nova_coluna = coluna + dj_1[direcao];

		if (posicao_valida(L, nova_linha, nova_coluna) &&
			L->celulas[nova_linha][nova_coluna] == '#' &&
			nova_coluna <= L->colunas) {
			L->celulas[nova_linha][nova_coluna] = ' ';
			feito++;
		}
	}

	// espelhando o labirinto;
	for (int i = 0; i < L->linhas; i++) {
		for (int j = L->colunas-1; j >= L->colunas / 2; j--) {
			L->celulas[i][j] = L->celulas[i][L->colunas - j - 1];
		}
	}
}

void resolve_bfs(labirinto* L) {
	L->distancia = (int**) malloc(L->linhas * sizeof(int*));
	for (int i = 0; i < L->linhas; i++) {
		L->distancia[i] = (int*) malloc(L->colunas * sizeof(int));
		for (int j = 0; j < L->colunas; j++) {
			L->distancia[i][j] = -1;
		}
	}
	L->distancia[1][1] = 0;

	Fila* F = Fila_();
	Fila_insere(F, 1, 1);
	while (!Fila_vazia(F)) {
		Nodo* agora = Fila_frente(F);
		int linha = agora->linha, coluna = agora->coluna;
		Fila_tira(F);

		for (int direcao = 0; direcao < 4; direcao++) {
			int nova_linha = linha + di_1[direcao], nova_coluna = coluna + dj_1[direcao];

			if (posicao_valida(L, nova_linha, nova_coluna) &&
				L->celulas[nova_linha][nova_coluna] != '#' &&
				L->distancia[nova_linha][nova_coluna] == -1) {

				L->distancia[nova_linha][nova_coluna] = L->distancia[linha][coluna] + 1;
				Fila_insere(F, nova_linha, nova_coluna);		
			}
		}
	}

	L->ordem = (int**)malloc(L->linhas * sizeof(int*));
	for (int i = 0; i < L-> linhas; i++) {
		L->ordem[i] = (int*)malloc(L->colunas * sizeof(int));
		for (int j = 0; j < L->colunas; j++) {
			L->ordem[i][j] = L->distancia[i][j];
		}
	}

	free(F);
	for (int i = 0; i < L->linhas; i++) {
		free(L->distancia[i]);
		free(L->ordem[i]);
	}
	free(L->distancia);
	free(L->ordem);
}

void resolve_dfs(labirinto* L) {
    L->distancia = (int**)malloc(L->linhas * sizeof(int*));
    L->ordem = (int**)malloc(L->linhas * sizeof(int*));
    for (int i = 0; i < L->linhas; i++) {
        L->distancia[i] = (int*)malloc(L->colunas * sizeof(int));
        L->ordem[i] = (int*)malloc(L->colunas * sizeof(int));
        for (int j = 0; j < L->colunas; j++) {
            L->distancia[i][j] = -1;
            L->ordem[i][j] = -1;
        }
    }
    L->distancia[1][1] = 0;
    int quantidade_vistos = 0;

    Pilha* P = Pilha_();
    Pilha_insere(P, 1, 1);
    while (!Pilha_vazia(P)) {
        Nodo* agora = Pilha_topo(P);
        int linha = agora->linha, coluna = agora->coluna;
        Pilha_tira(P);
        L->ordem[linha][coluna] = quantidade_vistos++;

        for (int direcao = 0; direcao < 4; direcao++) {
            int nova_linha = linha + di_1[direcao], nova_coluna = coluna + dj_1[direcao];

            if (posicao_valida(L, nova_linha, nova_coluna) &&
                L->celulas[nova_linha][nova_coluna] != '#' &&
                L->distancia[nova_linha][nova_coluna] == -1) {

                L->distancia[nova_linha][nova_coluna] = L->distancia[linha][coluna] + 1;
                Pilha_insere(P, nova_linha, nova_coluna);        
            }
        }
    }

    for (int i = 0; i < L->linhas; i++) {
        free(L->distancia[i]);
        free(L->ordem[i]);
    }
    free(L->distancia);
    free(L->ordem);
}

