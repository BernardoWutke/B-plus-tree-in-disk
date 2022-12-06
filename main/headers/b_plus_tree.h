#ifndef BPlusTree_H
#define BPlusTree_H
#include"../headers/util.h"

typedef struct  {
  int ordem, qtde, raiz;
} BP_Tree;


void inicializarBP();
void imprimirArvoreHeader();
int  buscarElemento(int id, int* indexPagina);
void inserirPaciente(Paciente a);
int  removerPaciente(Paciente a, BP_Tree bp_tree);

#endif
