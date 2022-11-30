#define _GNU_SOURCE
#include "../headers/arquivo.h"
#include "../headers/util.h"

void
inicializarTabela() {
  FILE* tabelaReg = fopen(REG_FILE, "wb");

  if (tabelaReg == NULL) {
    perror("Erro ao abrir arquivo");
    EXIT_FAILURE;
  }
  
  Tabela tab;
  tab.qtde = 0;

  fwrite(&tab, sizeof(Tabela), 1, tabelaReg);
  fclose(tabelaReg);
}


void 
criarTabelaRegistros (char* caminho) {
  inicializarTabela();
  
  FILE *arquivoTXT = fopen(caminho, "r");
  FILE *tabelaReg  = fopen(REG_FILE, "wb");

  if (tabelaReg == NULL || arquivoTXT == NULL) {
    perror("Erro ao abrir arquivo");
    EXIT_FAILURE;
  }
  
  size_t len = 100;
  char* registro = mallocSafe(len);
  const char delim[2] = ",";
  char *info;
  
  Tabela cabecalho;
  cabecalho.qtde = 0;

  fseek(tabelaReg, sizeof(Tabela), SEEK_SET);

  while (getline(&registro, &len, arquivoTXT) > 0) {
    int campo = 0;
    Artigo artigo;
    
    info = strtok(registro, delim);
    while (info != NULL) {
      switch (campo) {
        case 0: artigo.id = atoi(info); break;
        case 1: artigo.ano = atoi(info); break;
        case 2: strncpy(artigo.autor, info, TAM); break;
        case 3: strncpy(artigo.titulo, info, TAM); break;
        case 4: strncpy(artigo.revista, info, TAM); break;
        case 5: strncpy(artigo.DOI, info, TAM); break;
        case 6:
          strncpy(artigo.palavraChave, info, TAM);
          strcat(artigo.palavraChave, ",");
          break;
        default:
          strcat(artigo.palavraChave, info);
          strcat(artigo.palavraChave, ",");
          break;
      }
      
      campo++;
      info = strtok(NULL, delim);
    }  
    
    artigo.palavraChave[strlen(artigo.palavraChave) - 1] = '\0';
    
    fwrite(&artigo, sizeof(Artigo), 1, tabelaReg);
    cabecalho.qtde++;
  }
  
  rewind(tabelaReg);
  fwrite(&cabecalho, sizeof(Tabela), 1, tabelaReg);
  
  free(registro);
  fclose(arquivoTXT);
  fclose(tabelaReg);
}


void importarTabela () {
  FILE *tabelaReg = fopen(REG_FILE, "rb");
  
  if (tabelaReg == NULL) {
    perror("Erro ao abrir arquivo");
    EXIT_FAILURE;
  }
  
  BPTreeInicializar();

  Tabela cabecalho;
  Artigo artigo;
  
  fread(&cabecalho, sizeof(Tabela), 1, tabelaReg);

  for(int i = 0; i < cabecalho.qtde; i++){
    fread(&artigo, sizeof(Artigo), 1, tabelaReg);
    BPTreeInserir(artigo.id, i);
  }

  fclose(tabelaReg);
}
