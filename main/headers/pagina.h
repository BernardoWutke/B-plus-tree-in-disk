#ifndef _PAGE_H_
#define _PAGE_H_

#include"util.h"

typedef enum { 
  INTERNA, 
  FOLHA
} TipoPagina;

typedef struct pagina {
  int chave[ORDEM];
  int filho[ORDEM + 1];
  int ordem;
  int pai;
  int indexProximaPagina;
  int indexPaginaAnterior;
  int nivel;
  int qtdElementos;
  int index;
  TipoPagina tipo;

} Pagina;

Pagina *criaPagina(int ordem);
void inicializarPagina(Pagina *pagina, int ordem, int tipo);
void destroiPagina(Pagina *p);
void inserirElemento(Pagina *p, int chave);
void removerElemento(Pagina *p, int chave);
void verificarOverflow(Pagina *p);
void verificarUnderflow(Pagina *p);
void ordenarPagina(Pagina *p, int);




#endif /* _PAGE_H_ */