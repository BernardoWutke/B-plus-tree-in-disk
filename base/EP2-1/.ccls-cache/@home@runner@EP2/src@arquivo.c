#include "../headers/bplustree.h"
#include "../headers/util.h"

int
BPTreeContarElementos (char *filepath) {
  FILE *path = fopen(filepath, "r");

  if (path == NULL) { return -1; }
  
  char count = 0;
  int registro;
  
  while (true) { //!feof(path)
    if (feof(path)) { break; }
    if (fscanf(path, "%i", &registro)) {
      count++;
    }
  }

  fclose(path);
  
  return count;
}


int *
BPTreeCarregarArquivo (char *filepath) {
  FILE *path = fopen(filepath, "rb");

  if (path == NULL) { return NULL; }

  int size = BPTreeContarElementos(filepath);

  int *tabela = calloc (size, sizeof(int));
  int registro;

  for (int i = 0; i < size; i++) {
    if (feof(path)) { break; }
    if (fscanf(path, "%i", &registro)) {
      tabela[i] = registro;
    }
	}
  
  fclose(path);
  return tabela;
}


void 
BPTreeInserirTabela (BPTree *arvore, int *tabela, int size) {
  if (tabela == NULL) { return; }
  
  for (int i = 0; i < size; i++) {
    BPTreeInserir(arvore, &tabela[i]);
	}
}


int
BPTreeTamanhoTabela (char *filepath) {
  return BPTreeContarElementos(filepath);
}


void 
BPTreeLiberarArquivo (int *tabela) {
  free(tabela);
  tabela = NULL;
}