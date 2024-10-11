#ifndef arquivos_h
#define arquivos_h
#include <stdio.h>
#include <conio.h>

void salvar_arquivo(labirinto L){
    FILE *arq = fopen("mapas/mapas.txt", "w");
	fprintf(arq, "%d\n%d\n%c", L.linhas,L.colunas,L.celulas);
	fclose(arq);
}

void salvar_bin(labirinto L){
    FILE *arq = fopen("mapas/mapas.bin", "wb");
    fwrite(L.linhas, sizeof(int),1,arq);
    fwrite(L.colunas, sizeof(int),1,arq);
    fwrite(L.celulas, sizeof(int),1,arq);
}

void usuario_bin(labirinto L){
    arq = fopen("mapas/mapas.bin", "wb");
	fwrite(nome, sizeof(char[]),1,arq);
    fwrite(pontuacao, sizeof(int),1,arq);
}

#endif
