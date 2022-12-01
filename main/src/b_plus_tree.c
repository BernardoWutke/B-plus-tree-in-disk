#include"../headers/b_plus_tree.h"

void inicializarBP(int ordem){
    BP_Tree *bp = (BP_Tree*)malloc(sizeof(BP_Tree));
    bp->ordem = ordem;
    bp->qtde = 0;
    bp->raiz = 0;
    

<<<<<<< Updated upstream
void inicializarBP(BP_Tree *bp){
    // Tenta ler o arquivo base
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");
=======
  FILE *fp = fopen(ARQUIVO_ARVORE, "wb");
>>>>>>> Stashed changes

    // Se já tiver arquivo, carrega o cabeçalho em bp
    if (arquivoArvore != NULL) {
        fread(&bp, sizeof(BP_Tree), 1, arquivoArvore);
        fclose(arquivoArvore);
    }
    else{ // Se não cria um novo arquivo com um cabeçalho do zero
        bp->ordem = ORDEM;
        bp->qtde = 0;
        bp->raiz = -1;
        arquivoArvore = fopen(ARQUIVO_ARVORE, "wb+");
        fwrite(&bp, sizeof(BP_Tree), 1, arquivoArvore);
        fclose(arquivoArvore);
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

