#ifndef arquivos_h
#define arquivos_h
#include <stdio.h>
#include <conio.h>

void salvar_bin(labirinto L){
    FILE *arq = fopen("mapas/mapas.bin", "wb");
    fwrite(L.linhas, sizeof(int),1,arq);
    fwrite(L.colunas, sizeof(int),1,arq);
    fwrite(L.celulas, sizeof(celulas[][]),1,arq);
}

void ler_bin(labirinto L){
	arq = fopen("mapaa/mapas.bin", "rb");
    fread(L.linhas, sizeof(int),1,arq);
    fread(L.colunas, sizeof(int),1,arq);
    fread(L.celulas, sizeof(celulas[][]),1,arq);
}

void salvar_arquivo(labirinto L){
    FILE *arq = fopen("mapas/mapas.txt", "w");
	fprintf(arq, "%d\n%d\n%c", L.linhas,L.colunas,L.celulas);
	fclose(arq);
}

void usuario_txt(labirinto L){
	FILE *arq = fopen("mapas/usuarios.txt", "w");
	fprintf(arq, "%c\n%d", nome[],pontuacao);
	fclose(arq);
}

void ler_usuarios(labirinto L){
	arq = fopen("mapas/usuarios.txt", "r");
	fgets(nome,sizeof(nome[]),arq);
	fgets(pontuacao,sizeof(int),arq);
	fclose(arq);
}

#endif
