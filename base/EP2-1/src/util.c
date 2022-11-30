#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../headers/util.h"

#include "../headers/bplustree.h"
#include "../headers/pagina.h"

#define ERROR(msg) fprintf(stderr, "[!]: %s\n", #msg)

void *
mallocSafe (size_t nbytes) {
  void *ptr = malloc (nbytes);
  
  if (ptr == NULL) {
    ERROR(erro ao alocar memória);
    exit(EXIT_FAILURE);
  }

  return ptr;
}

  
void
imprimirArtigo(int id) {
  int posPag, posArt = -1;  
  
  if(!BPTreeProcurarElemento(id, &posPag)){
    printf("Artigo não encontrado!!!\n");
    return;
  }
  
  FILE *arquivoArvore = fopen(TREE_FILE, "rb");
  
  Pagina pag;
  fseek(arquivoArvore, sizeof(BPTree) + (posPag * sizeof(Pagina)), SEEK_SET);
  fread(&pag, sizeof(Pagina), 1, arquivoArvore);
  
  for(int i = 0; i < pag.qtde; i++){
    if(pag.chave[i] == id){
      posArt = pag.filho[i];
    }
  }

  fclose(arquivoArvore);
  
  FILE *tabelaReg = fopen(REG_FILE, "rb");

  if (tabelaReg == NULL) {
    perror("Erro ao abrir arquivo");
    EXIT_FAILURE;
  }
  
  fseek(tabelaReg, sizeof(Tabela) + (posArt * sizeof(Artigo)), SEEK_SET);
  Artigo artigo;
  fread(&artigo, sizeof(Artigo), 1, tabelaReg);
  fclose(tabelaReg);

  printf("\nId.........: %d", artigo.id);
  printf("\nAno........: %d", artigo.ano);
  printf("\nAutor......: %s", artigo.autor);
  printf("\nTitulo.....: %s", artigo.titulo);
  printf("\nRevista....: %s", artigo.revista);
  printf("\nDOI........: %s", artigo.DOI);
  printf("\nKeywords...: %s\n", artigo.palavraChave);
}