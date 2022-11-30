#ifndef BPLUSTREE_H_
#define BPLUSTREE_H_

#include "util.h"
#include "pagina.h"

typedef struct bplustree {
  int ordem, qtde;
  Pagina *raiz;
} BPTree;

BPTree *BPTreeInicializar();
BPTree *BPTreeInserir(BPTree *, int *);
BPTree *BPTreeRemover(BPTree *, int *);

Pagina *BPTreeRaiz(BPTree *);
//Pagina *BPTreeProcurarElemento(BPTree *, int *);
bool BPTreeProcurarElemento(BPTree*, Pagina**, int*);

void BPTreeLiberarMemoriaPaginas (Pagina *);
void BPTreeLiberarMemoria(BPTree *);
void BPTreeAtualizaNivel(Pagina *pag);
//void BPTreeExibirArvore(BPTree *);


int *BPTreeCarregarArquivo(char *);
int BPTreeTamanhoTabela(char *);
void BPTreeInserirTabela(BPTree *, int *, int);
void BPTreeLiberarArquivo(int *);
void BPTreeImprimeNivel(Pagina*);
void BPTreeImprimeConteudo(Pagina *);
#endif