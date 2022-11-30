#include "../headers/bplustree.h"
#include "../headers/pagina.h"

/*
 * @descrição: função que inicializa os campos da estrutura 
 * de B+Tree e escreve no arquivo de árvore os respectivos campos
 */
void 
BPTreeInicializar () {
  BPTree arvore;
  arvore.ordem   = D;
  arvore.qtde    = 0;
  arvore.raiz    = -1;
  arvore.indexes = 0;
  
  FILE *arquivoArvore = fopen(TREE_FILE, "wb");
  
  fwrite(&arvore, sizeof(BPTree), 1, arquivoArvore);
  fclose(arquivoArvore);
}


/*
 * @descrição: função que procura um elemento na árvore
 * @param chave: chave para realizar a busca
 * @param *pagIndex: recebe o index da página onde encontrou ou não a chave
 * @retorno: retorna true ou false se o elemento for encontrado    
 */
bool 
BPTreeProcurarElemento (int chave, int *pagIndex) {
  FILE *arquivoArvore = fopen(TREE_FILE, "rb");    
  // Realiza a leitura do cabeçalho da árvore
  BPTree arvore;
  
  fread(&arvore, sizeof(BPTree), 1, arquivoArvore);
  
  // Retorna false quando a arvore não tem uma raiz
  if(arvore.raiz < 0){
    *pagIndex = -1;
    fclose(arquivoArvore);
    return false;
  }
  
  // Realiza a leitura da raiz da árvore
  Pagina pag;
  fseek(arquivoArvore, sizeof(Pagina) * arvore.raiz, SEEK_CUR);
  fread(&pag, sizeof(Pagina), 1, arquivoArvore);

  // Loop que será executado caso a árvore não esteja vazia
  while (true) {
    if (ehFolha(pag)) {
      *pagIndex = pag.index; //Retorna por referência a posição da pagina folha
      for (int i = 0; i < pag.qtde; i++) {
        if (pag.chave[i] == chave) {
          fclose(arquivoArvore);
          return true;
        }
      }
      
      fclose(arquivoArvore);
      return false;
    } 
    
    else {
      for (int i = 0; i < pag.qtde; i++) {
        // Condição para verificar se a chave buscada for 
        // menor do que a primeira chave da pagina
        if (i == 0 && (pag.chave[i] > chave)) {
          // Realiza a leitura da primeira página filha
          fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pag.filho[0]), SEEK_SET);
          fread(&pag, sizeof(Pagina), 1, arquivoArvore);
          break;
        }
          
        // Condição para verificar se a chave buscada for menor
        // do que a chave na posição i
        else if (pag.chave[i] > chave) {
          // Realiza a leitura da página filha na posição i
          fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pag.filho[i]), SEEK_SET);
          fread(&pag, sizeof(Pagina), 1, arquivoArvore);
          break;
        }

        // Condição para verificar se a chave buscada é igual a chave i
        // em caso verdadeiro irá descer no filho na posição i + 1
        else if (pag.chave[i] == chave) {
          fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pag.filho[i + 1]), SEEK_SET);
          fread(&pag, sizeof(Pagina), 1, arquivoArvore);
          break;
        }

        // Se a busca chegou ao final das chaves na pagina
        // o passo seguinte será descer na pagina filha mais a direita
        else if (pag.qtde == (i + 1)) {
          fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pag.filho[i + 1]), SEEK_SET);
          fread(&pag, sizeof(Pagina), 1, arquivoArvore);
          break;
        }
      }
    }
  }
}

/*
 * @descrição: Aumenta o nível da arvore
 * @param pagina: Recebera uma página (raiz da arvore) para aumentar o nível de todos os seus descendentes 
 */
void BPTreeIncrementarNivel (FILE *arquivoArvore, Pagina *pag) {
  if(ehFolha(*pag)){
    pag->nivel++;

    fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pag->index), SEEK_SET);
    fwrite(pag, sizeof(Pagina), 1, arquivoArvore);
    
  } else {
    if(pag->pai > 0){
      fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pag->index), SEEK_SET);
      fwrite(pag, sizeof(Pagina), 1, arquivoArvore);
      pag->nivel++;
    }
    
    for(int i = 0; i < (pag->qtde + 1); i++){
      fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pag->filho[i]), SEEK_SET);
      
      Pagina aux;
      fread(&aux, sizeof(Pagina), 1, arquivoArvore);
      BPTreeIncrementarNivel(arquivoArvore, &aux);
    }
  }
}


/*
 * @descrição: realiza a inserção de uma chave na árvore
 * @param chave: chave que será usada para inserção
 */
void 
BPTreeInserir (int chave, int regIndex) {
  // pagIndex recebe o indice da última pagina visitada pela
  // função de busca (que irá ser uma folha) ou -1 se a árvore estiver vazia
  int pagIndex;
  
  if (BPTreeProcurarElemento(chave, &pagIndex)) {
    printf("Elemento ja esta na arvore\n");
    return;
  }
  
  FILE *arquivoArvore = fopen(TREE_FILE, "r+b");

  // Realiza a leitura do cabeçalho da árvore
  BPTree arvore;
  fread(&arvore, sizeof(BPTree), 1, arquivoArvore);

  // Condição que verifica se a árvore está vazia
  if (arvore.raiz < 0) {
    // Cria uma nova pagina raiz e atualiza o cabeçalho no arquivo
    Pagina *pagina = novaPagina(&arvore);
    inserirEmFolha(pagina, chave, regIndex);
    arvore.raiz = pagina->index;
    arvore.qtde++;
    rewind(arquivoArvore);
    fwrite(&arvore, sizeof(BPTree), 1, arquivoArvore);
    
    // Escreve a nova página no arquivo depois do cabeçalho
    fwrite(pagina, sizeof(Pagina), 1, arquivoArvore);
    printf("Pagina: %d\n", pagina->tipo);
    LiberarPagina(pagina);
    fclose(arquivoArvore);
    return;
  }

  // Ler a pagina folha onde a função de busca parou
  Pagina pagina;
  fseek(arquivoArvore, sizeof(Pagina) * pagIndex, SEEK_CUR);
  fread(&pagina, sizeof(Pagina), 1, arquivoArvore);
  
  // Inserir a nova chave nela
  inserirEmFolha(&pagina, chave, regIndex);
  arvore.qtde++;
  ordernarFolha(&pagina);

  fseek(arquivoArvore, -sizeof(Pagina), SEEK_CUR);
  fwrite(&pagina, sizeof(Pagina), 1, arquivoArvore);

  
  while (true) {
    if (verificarOverflow(&pagina)) {

      // Criar nova pagina irmã
      Pagina *paginaNova = novaPagina(&arvore);
      Pagina *paginaPai;
      
      // Se a pagina for a raiz (não tiver pai)
      if (pagina.pai < 0) {
        paginaPai = novaPagina(&arvore);
        arvore.raiz = paginaPai->index;
        paginaPai->filho[0] = pagina.index;
        pagina.pai = paginaPai->index;
        paginaPai->tipo = Interno;
        BPTreeIncrementarNivel(arquivoArvore, paginaPai);
      }
      // Se existir um pai, trazer ele para a memória
      else {
        fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pagina.pai), SEEK_SET);
        fread(paginaPai, sizeof(Pagina), 1, arquivoArvore);
      }
      
      if (ehFolha(pagina)) {
        // Tranferir chaves da pagina para a nova irmã
        for (int i = D; i < pagina.qtde; i++) {
          inserirEmFolha(paginaNova, pagina.chave[i], pagina.filho[i]);
        }
        
        // Colocar chave primeira chave da irmã no pai
        paginaPai->chave[paginaPai->qtde] = paginaNova->chave[0];
        paginaPai->qtde++;
        
        // Diminui contador de chaves em pagina (remoção lógica das chaves
        // passadas para a irmã)
        pagina.qtde -= paginaNova->qtde;

        //Atualiza a referência para a lista encadeada
        if (pagina.proximo > 0)
          paginaNova->proximo = pagina.proximo;
          pagina.proximo = paginaNova->index;
      }
      else {
        //Tranfere chaves da pagina interna para sua nova irmã
        for (int i = D + 1; i < pagina.qtde; i++) {
          paginaNova->chave[i - D - 1] = pagina.chave[i];
        }
        //Transferindo filhos da pagina interna para a nova irmã
        for (int i = D + 1; i < pagina.qtde + 1; i++) {
          paginaNova->filho[i - D - 1] = pagina.filho[i];
          fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * paginaNova->filho[i - D - 1]), SEEK_SET);
          //Alterando a referência do pai de cada filho
          Pagina *filho;
          fread(filho, sizeof(Pagina), 1, arquivoArvore);
          filho->pai = paginaNova->index;
          //Retirando a referência dos filhos tirandos de pagina
          pagina.filho[i] = -1;
          filho = NULL;
        }
        
        paginaPai->chave[paginaPai->qtde] = pagina.chave[D];
        paginaPai->qtde++;
        pagina.qtde -= paginaNova->qtde - 1;
        paginaNova->tipo = Interno;
      }

      paginaPai->filho[paginaPai->qtde] = paginaNova->index;
      paginaNova->pai = paginaPai->index;
      ordernarInterna(arquivoArvore, paginaPai);

      paginaNova->nivel = pagina.nivel;
      
      //escrever alterações no disco
      fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * pagina.index), SEEK_SET);
      fwrite(&pagina, sizeof(Pagina), 1, arquivoArvore);
      
      fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * paginaNova->index), SEEK_SET);
      fwrite(paginaNova, sizeof(Pagina), 1, arquivoArvore);

      fseek(arquivoArvore, sizeof(BPTree) + (sizeof(Pagina) * paginaPai->index), SEEK_SET);
      fwrite(paginaPai, sizeof(Pagina), 1, arquivoArvore);

      pagina = *paginaPai;

      // Se pagina pai tiver apenas uma chave, significa que
      // acabou de ser criada em um split e assim tem que ser desalocada
      // Nota: no caso contrário, a pagina pai é lida do disco e assim não é alocada
      if (paginaPai->qtde == 1) {
        free(paginaPai);
      }
      
      free(paginaNova);
      
    }
    else {
      break; 
    }
  }

  //salvar alterações no cabeçalho
  rewind(arquivoArvore);
  fwrite(&arvore, sizeof(BPTree), 1, arquivoArvore);
  fclose(arquivoArvore);
}


void 
BPTreeImprimeArvoreCompleta () {
  FILE *arquivoArvore = fopen(TREE_FILE, "r");

  // Lê o cabeçalho
  BPTree arvore;
  fread(&arvore, sizeof(BPTree), 1, arquivoArvore);
  // Verifica se a árvore está vazia
  if (arvore.qtde > 0) {
    // Lê a pagina raiz
    fseek(arquivoArvore, sizeof(BPTree) + (arvore.raiz * sizeof(Pagina)), SEEK_SET);
    Pagina pagina;
    fread(&pagina, sizeof(Pagina), 1, arquivoArvore);
    BPTreeImprimeNivel(&pagina, arquivoArvore);
  }

  fclose(arquivoArvore);
}

void 
BPTreeImprimeNivel (Pagina *pag, FILE *arquivoArvore) {
  if (pag->tipo == Externo) {
    // Função que imprime artigos
    printf("{%d} ", pag->index);
    printf("Folha ( ");
    for (int j = 0; j < pag->qtde; j++) {
      printf("%d ", pag->chave[j]);
    }
    printf(") [%d] ", pag->nivel);

    if (pag->proximo != -1) {
      fseek(arquivoArvore, sizeof(BPTree) + (pag->proximo * sizeof(Pagina)), SEEK_SET);
      Pagina proximo;
      fread(&proximo, sizeof(Pagina), 1, arquivoArvore);
      printf(" --> %d... | ", proximo.chave[0]);
    }
    printf("Pai: %d", pag->pai);
    printf("\n");
  } else {
    // Chama a função recursivamente para cada filho
    for (int i = 0; i < pag->qtde + 1; i++) {
      fseek(arquivoArvore, sizeof(BPTree) + (pag->filho[i] * sizeof(Pagina)), SEEK_SET);

      Pagina aux;
      fread(&aux, sizeof(Pagina), 1, arquivoArvore);
      BPTreeImprimeNivel(&aux, arquivoArvore);
    }
    printf("{%d} ", pag->index);
    printf("( ");
    for (int j = 0; j < pag->qtde; j++) {
      printf("%d ", pag->chave[j]);
    }
    printf(") [%d] ", pag->nivel);
    printf(" | Filhos -> ");
    for (int i = 0; i < pag->qtde + 1; i++){
      printf("%d ", pag->filho[i]);
    }
    printf("| Pai: %d", pag->pai);
    printf("\n");
  }
}