#ifndef PAGINA_H_
#define PAGINA_H_

#include "util.h"
#include "bplustree.h"

typedef enum { 
  INTERNA, 
  FOLHA 
} tFolha;

typedef struct pagina {
  int chave[MAX];
  int filho[MAX+1];
  int pai;
  int proximo;
  int nivel;
  int qtde;
  int index;
  tFolha tipo;
} Pagina;

void inicializarPagina(BPTree*, Pagina*);
void inserirEmFolha (Pagina*, int, int);
void ordenarFolha(Pagina*);
void ordenarInterna(FILE*, Pagina*);
tFolha ehFolha(Pagina);
bool verificarOverflow(Pagina *);
bool verificarUnderflow(Pagina *);
void liberarPagina(Pagina*);
void imprimirPagina(Pagina*);

#endif