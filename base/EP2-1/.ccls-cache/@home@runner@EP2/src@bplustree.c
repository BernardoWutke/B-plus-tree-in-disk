#include "../headers/bplustree.h"

BPTree *
BPTreeInicializar () {
  BPTree *arvore = mallocSafe(sizeof(BPTree));
  arvore->ordem  = D;
  arvore->qtde   = 0;
  arvore->raiz   = NULL;
  return arvore;
}


Pagina *
BPTreeRaiz(BPTree *arvore) {
  return arvore->raiz;
}

/*
Pagina * 
BPTreeProcurarElemento(BPTree *arvore, int *chave){
  Pagina *aux = arvore->raiz;

  while (aux != NULL) {
    if (ehNoFolha(aux)) {
      for(int i = 0; i < aux->qtde; i++){
        if(aux->chave[i] == *chave){
          return aux; 
        }
      }
      
      return NULL;
    } else {
      for(int i = 0; i < aux->qtde; i++){
        
        if(i == 0 && (aux->chave[i] > *chave)){
          aux = aux->filho[0];
          break;
        } else if(aux->chave[i] > *chave){
          aux = aux->filho[i];
          break;
        }else if(aux->chave[i] == *chave ){ 
          aux = aux->filho[i + 1];
          break;
        } else if(aux->qtde == (i+1)){
          aux = aux->filho[i + 1];
          break;
        }
      }
    }
  }
  return NULL;
}
*/

bool
BPTreeProcurarElemento(BPTree *arvore, Pagina** pag, int *chave){
  (*pag) = arvore->raiz;

  while ((*pag) != NULL) {
    if (ehNoFolha((*pag))) {
      for(int i = 0; i < (*pag)->qtde; i++){
        if((*pag)->chave[i] == *chave){
          return i+1;
        }
      } 
      return false;
    } else {
      for(int i = 0; i < (*pag)->qtde; i++){
        if(i == 0 && ((*pag)->chave[i] > *chave)){
          (*pag) = (*pag)->filho[0];
          break;
        } else if((*pag)->chave[i] > *chave){
          (*pag) = (*pag)->filho[i];
          break;
        }else if((*pag)->chave[i] == *chave ){ 
          (*pag) = (*pag)->filho[i + 1];
          break;
        } else if((*pag)->qtde == (i+1)){
          (*pag) = (*pag)->filho[i + 1];
          break;
        }
      }
    }
  }
  return false;
}

void
BPTreeOrdenarInt (int **chave, int qtde) {
  long temp;
  int j;

  for (int i = 1; i < qtde; i++) {
    temp = (int)chave[i];
    for (j = i; (j > 0) && (temp < (int)(chave[j - 1])); j--) {
      chave[j] = chave[j - 1];
    }

    chave[j] = (int*)temp;
  }
}

Pagina
*BPTreeOrdenarFilho(Pagina *pagina) {
  Pagina *temp = NULL;

  for (int i = 0; i < pagina->qtde + 1; i++) {
    for(int j = 0; j < pagina->qtde + 1; j++){
      if (pagina->filho[i]->chave[0] < pagina->filho[j]->chave[0]) {
        temp = pagina->filho[i];
        pagina->filho[i] = pagina->filho[j];
        pagina->filho[j] = temp;
      }
    }
  }

  
  return pagina;
}

void BPTreeIncrementaNivel (Pagina *pag) {
  if(ehNoFolha(pag)){
    pag->nivel++;
  } else {
    if(pag->pai) pag->nivel++;

    for(int i = 0; i < (pag->qtde + 1); i++){
      BPTreeIncrementaNivel(pag->filho[i]);
    }
  }
}

void BPTreeDecrementaNivel(Pagina *pag){
  if(ehNoFolha(pag)){
    pag->nivel--;
  } else {
    if(pag->pai) pag->nivel--;

    for(int i = 0; i < (pag->qtde + 1); i++){
      BPTreeDecrementaNivel(pag->filho[i]);
    }
  }
}

// Inserção de chaves int
BPTree *
BPTreeInserir(BPTree *arvore, int *chave){
  Pagina *aux;
  if(BPTreeProcurarElemento(arvore, &aux, chave)){
    printf("Elemento já está na arvore\n");
    return arvore;
  }
  
  // Caso a arvore esteja vazia
  if(aux == NULL){
    Pagina *pagina = novaPagina();
    memcpy(pagina->chave, chave, sizeof(int));
    pagina->qtde++;
    arvore->raiz = pagina;
    arvore->qtde++;
    return arvore;
  }

  memcpy(&aux->chave[aux->qtde], chave, sizeof(int));
  aux->qtde++;
  arvore->qtde++;
  BPTreeOrdenarInt(aux->chave, aux->qtde);
      
  // Trata overflow
  while (aux != NULL) {
    
    if(verificarOverflow(aux)){
      Pagina *pagina_pai = aux->pai;

      Pagina *pagina_nova = novaPagina();
      
      if(aux->pai == NULL){
        pagina_pai = novaPagina();
        arvore->raiz = pagina_pai;
        pagina_pai->filho[pagina_pai->qtde] = aux;
        aux->pai = pagina_pai;
        pagina_pai->tipo = Interno;
        // ATUALIZAR NÍVEL
        BPTreeIncrementaNivel(pagina_pai);
      }
      
      int medio = D; 
      
      // Transferir metade para um novo vetor filhos e chaves
      if(ehNoFolha(aux)){
        
        for(int i = medio; i < aux->qtde; i++){
          pagina_nova->chave[i - medio] = aux->chave[i];
          pagina_nova->qtde++;
        }
        
        pagina_pai->chave[pagina_pai->qtde] = pagina_nova->chave[0];
        pagina_pai->qtde++;
        
        // Atualiza vetor antigo
        aux->qtde -= pagina_nova->qtde;

      } else {
        for(int i = medio + 1; i < aux->qtde; i++){
          pagina_nova->chave[i - medio - 1] = aux->chave[i];
          pagina_nova->qtde++;
        }

        for(int i = medio + 1; i < (aux->qtde + 1); i++){
          pagina_nova->filho[i - medio - 1] = aux->filho[i];
          pagina_nova->filho[i - medio - 1]->pai = pagina_nova;
          aux->filho[i] = NULL;
        }
        
        pagina_pai->chave[pagina_pai->qtde] = aux->chave[medio];
        pagina_pai->qtde++;
        aux->qtde = aux->qtde - pagina_nova->qtde - 1;
      }
      
      pagina_pai->filho[pagina_pai->qtde] = pagina_nova;
      pagina_nova->pai = pagina_pai;
      BPTreeOrdenarInt(pagina_pai, pagina_pai->qtde);
      BPTreeOrdenarFilho(pagina_pai);
  
      // Ponteiro irmao
      if(ehNoFolha(aux)){
        if(aux->proximo != NULL){
          pagina_nova->proximo = aux->proximo;
        }
        aux->proximo = pagina_nova;
      } else {
        pagina_nova->tipo = Interno;
      }
      
      // Atualiza nível
      pagina_nova->nivel = aux->nivel;
      
      aux = aux->pai;  
    } else {
      break;
    }
  }
  
  return arvore;
}

BPTree *BPTreeRemover(BPTree *arvore, int *chave){
  if(arvore == NULL || arvore->raiz == NULL){
    printf("Árvore vazia\n");
    return arvore;
  }
  
  Pagina *aux;
  int posRemovida = BPTreeProcurarElemento(arvore, &aux, chave);
  if(posRemovida == false){
    printf("Elemento não está na arvore\n");
    return arvore;
  }


  // Remove elemento
  posRemovida--;
  aux->chave[posRemovida] = aux->chave[aux->qtde - 1];
  aux->qtde--;
  arvore->qtde--;
  BPTreeOrdenarInt(aux->chave, aux->qtde);

  int posNoPai = 0;
  for(int i = 0; i < aux->pai->qtde; i++){
      if(*chave < aux->pai->chave[i]){
      posNoPai = i;
      break;
    }
  }
  // VERIFICAR EM CASO DE ERROS
  // Atualiza Nó pai
  if (posRemovida){
    if(!(aux == aux->pai->filho[0])){
      aux->pai->chave[posNoPai] = aux->chave[0];
    }
  }  

  while(aux != NULL && (aux != arvore->raiz)){  
    if(verificarUnderflow(aux)){
      if(ehNoFolha(aux)){

        //Se a página com underflow for a primeira filha (não tem irmã a esquerda)
        if (aux == aux->pai->filho[0]){
          Pagina *irmao = aux->proximo;
          //Se o irmão após uma remoção ficar com underflow
          if((irmao->qtde - 1) < D){
            
            //Merge: passar as chaves de irmão para aux
            for (int i = 0; i < irmao->qtde; i++){
              aux->chave[aux->qtde + i] = irmao->chave[i];
            }
            aux->qtde += irmao->qtde;
            aux->proximo = irmao->proximo;
            aux->pai->chave[0] = aux->pai->chave[aux->pai->qtde - 1];
            aux->pai->filho[1] = aux->pai->filho[aux->pai->qtde];
            BPTreeOrdenarInt(aux->pai, aux->pai->qtde);
            BPTreeOrdenarFilho(aux->pai);
            aux->pai->qtde--;
            LiberaPagina(irmao);          
          }
          else{
            //Passar elemento do irmão a direita para a página
            aux->chave[aux->qtde] = irmao->chave[0];
            aux->qtde++;
            irmao->qtde--;
            irmao->chave[0] = irmao->chave[irmao->qtde];  
            BPTreeOrdenarInt(irmao->chave, irmao->qtde);

            //Atualizar a nova menor chave do irmão no pai
            aux->pai->chave[0] = irmao->chave[0];
          } 
        }
        else if (aux == aux->pai->filho[aux->pai->qtde]) {
          Pagina* irmao = aux->pai->filho[aux->pai->qtde-1];
          
          if (irmao->qtde - 1 < D){

            //Merge: passar as chaves de irmão para aux
            for (int i = 0; i < aux->qtde; i++){
              irmao->chave[irmao->qtde + i] = aux->chave[i];
            }
            
            irmao->proximo = NULL;
            irmao->qtde += aux->qtde;
            aux->pai->qtde--;

            LiberaPagina(aux); 
            aux = irmao;
          }
          else{
            //Passar elemento do irmão a esquerda para a página
            aux->chave[aux->qtde] = irmao->chave[irmao->qtde-1];
            aux->qtde++;
            irmao->qtde--;
            BPTreeOrdenarInt(aux->chave, aux->qtde);
            //Atualiza chave no pai
            aux->pai->chave[aux->pai->qtde-1] = aux->chave[0];
          }
        }
        else{

          //Se o irmão da esquera não ficar com underflow após doar uma chave
          if(aux->pai->filho[posNoPai - 1]->qtde - 1 >= D){
            
            Pagina* irmao = aux->pai->filho[posNoPai - 1];

            aux->chave[aux->qtde] = irmao->chave[irmao->qtde - 1];
            aux->qtde++;
            irmao->qtde--;
            BPTreeOrdenarInt(aux->chave, aux->qtde);
            aux->pai->chave[posNoPai - 1] = aux->chave[0];

          } 
          //Se o irmão da direita não ficar com underflow após doar uma chave
          else if (aux->pai->filho[posNoPai + 1]->qtde - 1 >= D){
            Pagina* irmao = aux->pai->filho[posNoPai + 1];

            aux->chave[aux->qtde] = irmao->chave[0];
            aux->qtde++;
            irmao->qtde--;

            //Ordenando o irmão após ele doar a primeira chave
            irmao->chave[0] = irmao->chave[irmao->qtde];
            BPTreeOrdenarInt(irmao->chave, irmao->qtde);
            irmao->pai->chave[posNoPai] = irmao->chave[0];
          }
          //Caso não de para pegar uma chave dos irmãos, faz o merge
          else {
            Pagina *irmao = aux->pai->filho[posNoPai - 1];
            
            //Merge: passar as chaves de aux para irmão
            for (int i = 0; i < aux->qtde; i++){
              irmao->chave[irmao->qtde + i] = aux->chave[i];
            }
            
            irmao->proximo = aux->proximo;
            irmao->qtde += aux->qtde;
            
            //Removendo do pai a chave da página combinada
            aux->pai->chave[posNoPai - 1] = aux->pai->chave[posNoPai];
            
            aux->pai->filho[posNoPai] = aux->pai->filho[aux->pai->qtde];
            BPTreeOrdenarInt(aux->pai, aux->pai->qtde);
            BPTreeOrdenarFilho(aux->pai);
            aux->pai->qtde--;
            LiberaPagina(aux);
            aux = irmao;
          }
        }
      }
      //Se a página com underflow não for uma folha
      else{
        //Se for a página interna mais a esquerda no nível
        if (aux == aux->pai->filho[0]){
          Pagina *irmao = aux->pai->filho[1];
          //Se o irmão após uma remoção ficar com underflow
          if((irmao->qtde - 1) < D){
            
            //Descer a menor chave do pai
            aux->chave[aux->qtde] = aux->pai->chave[0];
            aux->qtde++;
            
            //Merge: passar as chaves de irmão para aux
            for (int i = 0; i < irmao->qtde; i++){
              aux->chave[aux->qtde + i] = irmao->chave[i];
            }
            //Passar os filhos do irmão para aux
            for (int i = 0; i < irmao->qtde+1; i++){
              aux->filho[aux->qtde + i] = irmao->filho[i];
            }
            
            aux->qtde += irmao->qtde;
            LiberaPagina(irmao);
            
            //Se o pai após descer a chave ficar vazio
            if((aux->pai->qtde - 1) == 0 && aux->pai->pai == NULL){
              arvore->raiz = aux;
              LiberaPagina(aux->pai);
              aux->pai = NULL;
              aux->nivel = 0;
              BPTreeDecrementaNivel(arvore->raiz);
            } else {
              aux->pai->chave[0] = aux->pai->chave[aux->pai->qtde - 1];
              aux->pai->filho[1] = aux->pai->filho[aux->pai->qtde];
              aux->pai->qtde--;
              BPTreeOrdenarInt(aux->pai, aux->pai->qtde);
              BPTreeOrdenarFilho(aux->pai);
            }
          }
          //Se o irmão puder doar uma chave
          else{   
            aux->chave[aux->qtde] = aux->pai->chave[0];
            aux->qtde++;
            aux->pai->chave[0] = irmao->chave[0];
            BPTreeOrdenarInt(aux->pai, aux->pai->qtde);
                        
            aux->filho[aux->qtde] = irmao->filho[0];
            irmao->filho[0] = irmao->filho[irmao->qtde];
            irmao->chave[0] = irmao->chave[irmao->qtde - 1];
            irmao->qtde--;
            BPTreeOrdenarInt(irmao, irmao->qtde);
            BPTreeOrdenarFilho(irmao);
          }
        }
        //Se a página interna mais a direita do nível
        else if(aux == aux->pai->filho[aux->pai->qtde]){

          //Pegar irmão a direita pois não há a esquerda
          Pagina* irmao = aux->pai->filho[aux->pai->qtde - 1];
          if (irmao->qtde - 1 < D){
            
            //Descer a menor chave do pai
            irmao->chave[irmao->qtde] = irmao->pai->chave[irmao->pai->qtde - 1];
            irmao->qtde++;
          
            //Merge: passar as chaves de aux para irmao
            for (int i = 0; i < aux->qtde; i++){
              irmao->chave[irmao->qtde + i] = aux->chave[i];
            }
            //Passar os filhos do irmão para aux
            for (int i = 0; i < aux->qtde+1; i++){
              irmao->filho[irmao->qtde + i] = aux->filho[i];
            }
            
            irmao->qtde += aux->qtde;
            LiberaPagina(aux);

            //Se o pai após descer a chave ficar vazio
            if((irmao->pai->qtde - 1) == 0 && irmao->pai->pai == NULL){
              arvore->raiz = irmao;
              LiberaPagina(irmao->pai);
              irmao->pai = NULL;
              irmao->nivel = 0;
              BPTreeDecrementaNivel(arvore->raiz);
            } else {
              irmao->pai->qtde--;
            }
            aux = irmao;
  
          }
          else{
            aux->chave[aux->qtde] = aux->pai->chave[aux->pai->qtde - 1];
            aux->qtde++;
            BPTreeOrdenarInt(aux, aux->qtde);
            aux->pai->chave[aux->pai->qtde - 1] = irmao->chave[irmao->qtde - 1];
            aux->filho[aux->qtde] = irmao->filho[irmao->qtde];
            irmao->qtde--;
            BPTreeOrdenarFilho(aux);
          }
        }
        else{

          if(aux->pai->filho[posNoPai - 1]->qtde - 1 >= D){
            Pagina* irmao = aux->pai->filho[posNoPai - 1];

            //Descer a chave do pai
            aux->chave[aux->qtde] = aux->pai->chave[posNoPai - 1];
            aux->qtde++;
            //Subir a chave do irmão para o pai
            aux->pai->chave[posNoPai - 1] = irmao->chave[irmao->qtde - 1];
            //Passar o último filho do irmão para o aux
            aux->filho[aux->qtde] = irmao->filho[irmao->qtde];
            irmao->qtde--;
            
            BPTreeOrdenarInt(aux->chave, aux->qtde);
            BPTreeOrdenarFilho(aux->chave);
          }
          //Se o irmão da direita não ficar com underflow após doar uma chave
          else if (aux->pai->filho[posNoPai + 1]->qtde - 1 >= D){
            Pagina* irmao = aux->pai->filho[posNoPai + 1];

            //Desce uma chave do pai para aux
            aux->chave[aux->qtde] = aux->pai->chave[posNoPai];
            aux->qtde++;
            //Sobe o primeiro filho do irmão para o lugar desta chave
            aux->pai->chave[posNoPai] = irmao->chave[0];
            irmao->chave[0] = irmao->chave[irmao->qtde-1];
            //Transfere para aux pagina sobrinha das chaves menores que a chave 
            //doada pelo irmão ao pai
            aux->filho[aux->qtde] = irmao->filho[0];
            //Consertando os filho e a as chaves do irmão
            irmao->filho[0] = irmao->filho[irmao->qtde];
            irmao->qtde--;
            BPTreeOrdenarInt(irmao->chave, irmao->qtde);
            BPTreeOrdenarFilho(irmao->chave);
          }
          //Se nenhum dos irmãos puder doar uma chave
          else{
            Pagina *irmao = aux->pai->filho[posNoPai - 1];

            //Descer chave do pai pro irmao
            irmao->chave[irmao->qtde] = irmao->pai->chave[posNoPai - 1];
            irmao->qtde++;
            
            //Se o pai após descer a chave ficar vazio
            if((irmao->pai->qtde - 1) == 0 && irmao->pai->pai == NULL){
              arvore->raiz = irmao;
              LiberaPagina(irmao->pai);
              irmao->pai = NULL;
              irmao->nivel = 0;
              BPTreeDecrementaNivel(arvore->raiz);
            } else {
              irmao->pai->chave[posNoPai -1] = irmao->pai->chave[irmao->pai->qtde - 1];
              irmao->pai->filho[posNoPai] = irmao->pai->filho[aux->pai->qtde];
              irmao->pai->qtde--;
              BPTreeOrdenarInt(irmao->pai, irmao->pai->qtde);
              BPTreeOrdenarFilho(irmao->pai);
            }

            //Merge: passar a chaves de aux para o irmao
            for(int i = 0; i < aux->qtde; i++){
              irmao->chave[irmao->qtde + i] = aux->chave[i];
            }

            //Passar os filhos de aux para o irmao
            for(int i = 0; i < aux->qtde + 1; i++){
              irmao->filho[irmao->qtde + i] = aux->filho[i];
            }
            
            irmao->qtde += aux->qtde;

            LiberaPagina(aux);
            aux = irmao;
          }
        }  
      }
      
      aux = aux->pai;
      
      if(aux->pai != NULL){
        for(int i = 0; i < aux->pai->qtde; i++){
          if(*chave < aux->pai->chave[i]){
            posNoPai = i;
            break;
          }
        }
      }
    }
    else
      break;
  }  
  
  arvore->qtde--;
  return arvore;
}
  
void
BPTreeLiberarMemoriaPaginas (Pagina *raiz){
  if(ehNoFolha(raiz)){
    free(raiz->filho);
    free(raiz);
  } else {
    for(int i = 0; i < (raiz->qtde + 1); i++){
      BPTreeLiberarMemoriaPaginas(raiz->filho[i]);
    }

    raiz->tipo = Externo;
    BPTreeLiberarMemoriaPaginas(raiz);
  }
}

void BPTreeImprimeNivel(Pagina* pag){
  if(ehNoFolha(pag)){
    printf("F( ");
    for (int j = 0; j < pag->qtde; j++){
      printf("%d ", pag->chave[j]);
    }
    printf(")");
    printf("[%d] ", pag->nivel);
    if (pag->proximo != NULL)
      printf(" --> %d...", pag->proximo->chave[0]);
    printf("\n");
  }
  else{
    for (int i = 0; i < pag->qtde + 1; i++){
      BPTreeImprimeNivel(pag->filho[i]);
    }
    printf("( ");
      for (int j = 0; j < pag->qtde; j++){
        printf("%d ", pag->chave[j]);
      }
      printf(")");
      printf("[%d] ", pag->nivel);
      printf("\n");
  } 
}

void BPTreeImprimeConteudo(Pagina *raiz){
  if(raiz == NULL){
    return;
  }
  if(ehNoFolha(raiz)){
    for(int i = 0; i < raiz->qtde; i++){
      printf("%d ", raiz->chave[i]);
    }
    BPTreeImprimeConteudo(raiz->proximo);
  }
  else{
    BPTreeImprimeConteudo(raiz->filho[0]);
  }
}
void
BPTreeLiberarMemoria(BPTree *arvore){
  if(arvore->raiz != NULL){
    BPTreeLiberarMemoriaPaginas(arvore->raiz);
  }
  
  free(arvore);
  arvore = NULL;
}

