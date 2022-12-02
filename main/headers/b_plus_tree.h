#ifndef BPlusTree_H
#define BPlusTree_H
#include"../headers/util.h"

typedef struct bp_tree {
  int ordem, qtde, raiz;
} BP_Tree;





void inicializarBP(int ordem);
void imprimirArvore();
int  buscarID(int);
int  inserirPaciente(Paciente a);
int  removerPaciente(Paciente a);

#endif
