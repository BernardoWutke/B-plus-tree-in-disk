#include "../headers/pagina.h"

void
inicializarPagina (BPTree* arvore, Pagina* pagina) {
  pagina->qtde    = 0;
  pagina->tipo    = FOLHA;
  pagina->nivel   = 0;
  pagina->index   = arvore->indexes;
  pagina->proximo = -1;
  pagina->pai     = -1;
  arvore->indexes++;
  
  for (int i = 0; i < MAX; i++){
    pagina->chave[i] = -1;
    pagina->filho[i] = -1;

    if((i + 1) == MAX){
      pagina->filho[i + 1] = -1;
    }
  }
}


void 
inserirEmFolha(Pagina* pagina, int chave, int regIndex) {
  if (pagina->tipo == FOLHA) {
    pagina->chave[pagina->qtde] = chave;
    pagina->filho[pagina->qtde] = regIndex;
    pagina->qtde++;
  }
  else {
    printf("\nERRO: Tentando inserir em pagina interna\n");
  }
}


void 
ordenarFolha(Pagina* pagina) {
  int j, temp1, temp2;
  
  for (int i = 1; i < pagina->qtde; i++) {
    temp1 = pagina->chave[i];
    temp2 = pagina->filho[i];
    for (j = i; j > 0 && temp1 < pagina->chave[j - 1]; j--) {
      pagina->chave[j] = pagina->chave[j - 1];
      pagina->filho[j] = pagina->filho[j - 1];
    }
    
    pagina->chave[j] = temp1;
    pagina->filho[j] = temp2;
  }
}


void
ordenarInterna (FILE *arquivoArvore, Pagina *pagina) {
  int j, temp;
  
  for (int i = 1; i < pagina->qtde; i++) {
    temp = pagina->chave[i];
    for (j = i; j > 0 && temp < pagina->chave[j - 1]; j--) {
      pagina->chave[j] = pagina->chave[j - 1];
    }
    
    pagina->chave[j] = temp;
  }
  
  temp = 0;
  
  Pagina filhoI, filhoJ;
  
  for(int i = 0; i < pagina->qtde + 1; i++){
    fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pagina->filho[i]), SEEK_SET);
    fread(&filhoI, sizeof(Pagina), 1, arquivoArvore);
  }

  for (int i = 0; i < pagina->qtde; i++) {
    //Ler o filho I
    fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pagina->filho[i]), SEEK_SET);
    fread(&filhoI, sizeof(Pagina), 1, arquivoArvore);
    for (j = i + 1; j < pagina->qtde + 1; j++) {   
      //Ler o filho J
      fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pagina->filho[j]), SEEK_SET);
      fread(&filhoJ, sizeof(Pagina), 1, arquivoArvore);
      //Se a primira chave do filho i fo menor do que a primeira do filho j
      //troca os filhos de lugar
      if (filhoJ.chave[0] < filhoI.chave[0]) {
        temp = filhoI.index;
        pagina->filho[i] = filhoJ.index;
        pagina->filho[j] = temp;
      }
    }
  }
}


tFolha
ehFolha (Pagina pagina) {
  return pagina.tipo;
}


bool 
verificarOverflow(Pagina *pagina){
  return pagina->qtde > (2 * D);
}


bool 
verificarUnderflow (Pagina *pagina) {
  return pagina->qtde < D;
}


void 
liberarPagina (Pagina *pagina) {
  free(pagina);
}


void 
imprimirPagina (Pagina *pagina) {
  printf("Chaves: ");
  for (int i = 0; i < pagina->qtde; i++){
    printf("%d | ", pagina->chave[i]);
  }
  printf("\nFilhos: ");
  for (int i = 0; i < pagina->qtde; i++){
    printf("%d | ", pagina->filho[i]);
  }
}


