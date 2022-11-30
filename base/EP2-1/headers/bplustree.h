#ifndef BPLUSTREE_H_
#define BPLUSTREE_H_

#include "arquivo.h"
#include "pagina.h"
#include "util.h"

typedef struct bptree {
  int ordem, qtde, raiz, indexes;
} BPTree;

void BPTreeInicializar();
bool BPTreeProcurarElemento(int, int*);
void BPTreeIncrementarNivel (FILE *, Pagina *);
void BPTreeInserir (int, int);
void BPTreeImprimeArvoreCompleta();
void BPTreeImprimeNivel(Pagina *, FILE *);
void BPTreeCorrigirOverflow(BPTree *arvore, Pagina pagina);

#endif