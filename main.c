#include <stdio.h>
#include "labirintos.h"

int main() {
	int sair = 0;
	while (!sair) {
		puts("[+] menu inicial.");
		puts("[0] sair.");
		puts("[1] (dedalo) criar labiririntos.");

		printf("[?] selecione uma das opcoes: ");
		int selecao;
		scanf("%d", &selecao);
		switch (selecao) {
			case 0:
				sair = 1;
				break;
			case 1:
				gera_labirinto();
				break;
			default:
				puts("[e] selecao invalida!");
		}
	}

	return 0;
}
