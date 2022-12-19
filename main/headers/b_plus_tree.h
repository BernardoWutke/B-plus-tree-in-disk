#ifndef BPlusTree_H
#define BPlusTree_H
#include"../headers/util.h"

typedef struct  {
  int ordem, qtdPaginas, raiz, qtdPacientes;
} BP_Tree;


void inicializarBP();
void imprimirArvore();
int  buscarPaciente(int id, int* indexPagina);
void inserirPaciente(Paciente a);
void deletarPaciente(int id);
int  removerPaciente(Paciente a, BP_Tree bp_tree);

#endif
