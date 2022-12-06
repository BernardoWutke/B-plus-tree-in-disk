#ifndef BPlusTree_H
#define BPlusTree_H
#include"../headers/util.h"

typedef struct bp_tree {
  int ordem, qtde, raiz;
} BP_Tree;


void inicializarBP(BP_Tree *bp);
void imprimirArvore();
int  buscarElemento(int id, int* indexPagina, BP_Tree bp_tree);
int  inserirPaciente(Paciente a, BP_Tree bp_tree);
int  removerPaciente(Paciente a, BP_Tree bp_tree);

#endif
