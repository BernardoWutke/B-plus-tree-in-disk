#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include "../headers/bplustree.h"


int main (void) {
  BPTree *arvore = BPTreeInicializar();
  int insertVec[] = {69, 52, 46, 40, 84, 67, 77, 27, 33, 5, 62, 79, 55, 37, 95, 17, 0, 85, 25, 99, 74, 92, 96, 2, 4, 53, 91, 81, 63, 35, 49, 14, 82, 90, 65, 32, 47, 3, 19, 93, 58, 57, 87, 76, 23, 26};

  srand(time(NULL));
  
  for (int i = 0; i < sizeof(insertVec)/sizeof(int); i++){
    BPTreeInserir(arvore, &insertVec[i]);
  }
  
  BPTreeImprimeNivel(arvore->raiz);

  printf("\n\n===== REMOCAO =====\n");
  
  int removeVec[] = {};
  for (int i = 0; i < sizeof(removeVec)/sizeof(int); i++){
    printf("\nREMOVENDO O %d\n", removeVec[i]);
    BPTreeRemover(arvore, &removeVec[i]);
    BPTreeImprimeNivel(arvore->raiz);
    printf("\n");
  }
  
  BPTreeLiberarMemoria(arvore);
  
  return EXIT_SUCCESS;
}