#include "estruturas.h"
#include <stdlib.h>

Fila* Fila_() {
    Fila* F = (Fila*)malloc(sizeof(Fila));
    F->frente = F->tras = NULL;
    return F;
}

void Fila_insere(Fila* F, int linha_, int coluna_) {
    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));
    novo->linha = linha_;
    novo->coluna = coluna_;
    novo->proximo = NULL;

    if (F->tras == NULL) {
        F->frente = F->tras = novo;
        return;
    }

    F->tras->proximo = novo;
    F->tras = novo;
}

void Fila_tira(Fila* F) {
    if (Fila_vazia(F)) return;

    Nodo* temp = F->frente;
    F->frente = F->frente->proximo;
    free(temp);

    if (F->frente == NULL) F->tras = NULL;
}

Nodo* Fila_frente(Fila* F) {
    if (Fila_vazia(F)) return NULL;

    return F->frente;
}

int Fila_vazia(Fila* F) {
    return (F->frente == NULL);
}

Pilha* Pilha_() {
    Pilha* P = (Pilha*)malloc(sizeof(Pilha));
    P->topo = NULL;
    return P;
}

void Pilha_insere(Pilha* P, int linha_, int coluna_) {
    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));
    novo->linha = linha_;
    novo->coluna = coluna_;
    novo->proximo = P->topo;
    P->topo = novo;
}

void Pilha_tira(Pilha* P) {
    if (Pilha_vazia(P)) return;

    Nodo* temp = P->topo;
    P->topo = P->topo->proximo;
    free(temp);
}

Nodo* Pilha_topo(Pilha* P) {
    if (Pilha_vazia(P)) return NULL;

    return P->topo;
}

int Pilha_vazia(Pilha* P) {
    return (P->topo == NULL);
}
