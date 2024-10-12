#ifndef estruturas_h
#define estruturas_h
#include <stdlib.h>

typedef struct Nodo {
    int linha;
    int coluna;
    struct Nodo* proximo;
} Nodo;

typedef struct {
    Nodo* frente;
    Nodo* tras;
} Fila;

typedef struct {
    Nodo* topo;
} Pilha;

Fila* Fila_();
void Fila_insere(Fila* F, int linha_, int coluna_);
void Fila_tira(Fila* F);
int Fila_vazia(Fila* F);
Nodo* Fila_frente(Fila* F);

Pilha* Pilha_();
void Pilha_insere(Pilha* P, int linha_, int coluna_);
void Pilha_tira(Pilha* P);
int Pilha_vazia(Pilha* P);
Nodo* Pilha_topo(Pilha* P);

#endif
