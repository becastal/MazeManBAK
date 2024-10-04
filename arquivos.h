#ifndef arquivos_h
#define arquivos_h
#include <stdio.h>
#include <conio.h>

void salvar_arquivo(labirinto L){
    FILE *arq = fopen("mapas/mapas.txt", "w");
	fprintf(arq, "%d %d %c", L.linhas,L.colunas,L.celulas);
	fclose(arq);
}

void salvar_bin(labirinto L){
    FILE *arq = fopen("mapas/mapas.txt", "wb");
    fwrite(L.linhas, sizeof(int),1,arq);
    fwrite(L.colunas, sizeof(int),1,arq);
    fwrite(L.celulas, sizeof(int),1,arq);
}

#endif
