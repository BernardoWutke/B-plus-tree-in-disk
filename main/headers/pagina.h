#ifndef _PAGE_H_
#define _PAGE_H_

#include"util.h"

typedef enum { 
  INTERNA, 
  FOLHA 
} TipoPagina;

typedef struct pagina {
  int *chave;
  int *filho;
  int ordem;
  int pai;
  int prox;
  int ant;
  int nivel;
  int qtde;
  int index;
  TipoPagina tipo;

} Pagina;

Pagina *criaPagina(int ordem);
void destroiPagina(Pagina *p);
void inserirElemento(Pagina *p, int chave);
void removerElemento(Pagina *p, int chave);
void verificarOverflow(Pagina *p);
void verificarUnderflow(Pagina *p);
void ordenarPagina(Pagina *p, int);




#endif /* _PAGE_H_ */