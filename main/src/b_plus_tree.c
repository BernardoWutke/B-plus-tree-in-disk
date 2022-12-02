#include"../headers/b_plus_tree.h"


    


void inicializarBP(BP_Tree *bp){
    // Tenta ler o arquivo base

  FILE *fp = fopen(ARQUIVO_ARVORE, "wb");
// Se já tiver arquivo, carrega o cabeçalho em bp
    if (fp != NULL) {
        fread(&bp, sizeof(BP_Tree), 1, fp);
        fclose(fp);
    }
    else{ // Se não cria um novo arquivo com um cabeçalho do zero
        bp->qtde = 0;
        bp->raiz = -1;
        fp = fopen(ARQUIVO_ARVORE, "wb+");
        fwrite(&bp, sizeof(BP_Tree), 1, fp);
        fclose(fp);
    }
}

void imprimirArvore(){
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

