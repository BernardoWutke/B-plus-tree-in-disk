#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_
#include<util.h>

typedef struct bp_tree {
  int ordem, qtde, raiz;
} BP_Tree;





void inicializarBP(BP_Tree *bp);
void imprimirArvore();
int  buscarID(int);
int  inserirPaciente(Paciente a);
int  removerPaciente(Paciente a);

#endif
