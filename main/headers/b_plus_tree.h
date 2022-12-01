#ifndef BPlusTree_H
#define BPlusTree_H
#include"../headers/util.h"

typedef struct bp_tree {
  int ordem, qtde, raiz;
} BP_Tree;





<<<<<<< Updated upstream
void inicializarBP(BP_Tree *bp);
=======
void inicializarBP(int ordem);
>>>>>>> Stashed changes
void imprimirArvore();
int  buscarID(int);
int  inserirPaciente(Paciente a);
int  removerPaciente(Paciente a);

<<<<<<< Updated upstream
#endif
=======
#endif 
>>>>>>> Stashed changes
