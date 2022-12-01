#include "../headers/util.h"
#include "../headers/b_plus_tree.h"


void imprimirArquivo(){
    FILE *fp = fopen(ARQUIVO_ARVORE, "rb");

  if (fp == NULL) {
    perror("Não foi possível abrir o arquivo !!!");
    EXIT_FAILURE;
  }
  
  BP_Tree *bp;
  fread(&bp, sizeof(BP_Tree), 1, fp);
  printf("Ordem: %d \n", bp->ordem);
  printf("Quantidade: %d \n", bp->qtde);
  printf("Raiz: %d \n", bp->raiz);
  fclose(fp);
}