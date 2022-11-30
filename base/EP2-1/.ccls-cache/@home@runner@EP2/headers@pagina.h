#ifndef PAGINA_H_
#define PAGINA_H_

#include "util.h"

typedef enum { 
  Interno, 
  Externo 
} tFolha;
  
typedef struct pagina {
  int *chave[MAX];
  int nivel;
  int qtde;
  struct pagina *pai; 
  struct pagina **filho;
  struct pagina *proximo;
  tFolha tipo;
} Pagina;

Pagina *novaPagina();
tFolha ehNoFolha(Pagina *);
bool verificarOverflow(Pagina *);
bool verificarUnderflow(Pagina *);

void LiberaPagina(Pagina*);

void ImprimePagina(Pagina*);

#endif