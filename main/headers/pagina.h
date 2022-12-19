#ifndef _PAGE_H_
#define _PAGE_H_

#include"util.h"

typedef enum { 
  INTERNA, 
  FOLHA
} TipoPagina;

typedef struct pagina {
  int chave[ORDEM + 1];
  int filho[ORDEM + 2];
  int ordem;
  int pai;
  int indexProximaPagina;
  int indexPaginaAnterior;
  int nivel;
  int qtdElementos;
  int index;
  int foiDeletada;
  TipoPagina tipo;

} Pagina;

Pagina *criaPagina(int ordem);
void inicializarPagina(Pagina *pagina, int ordem, int index, int tipo);
void destroiPagina(Pagina *p);
void inserirElemento(Pagina *p, int chave);
void removerElemento(Pagina *p, int chave);
void verificarOverflow(Pagina *p);
void verificarUnderflow(Pagina *p);
void ordenarPaginaFolha(Pagina *p);




#endif /* _PAGE_H_ */