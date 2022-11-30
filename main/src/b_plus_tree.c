#include<b_plus_tree.h>

void inicializarBP(BP_Tree *bp, int ordem){
  bp->ordem   = ordem;
  bp->qtde    = 0;
  bp->raiz    = -1;
  
  
  FILE *fp = fopen(ARQUIVO_ARVORE, "wb");

  if (fp == NULL) {
    perror("Não foi possível abrir o arquivo !!!");
    EXIT_FAILURE;
  }
  
  fwrite(&bp, sizeof(BP_Tree), 1, fp);
  fclose(fp);
}

