#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "../headers/bplustree.h"
#include "../headers/pagina.h"

int main (int argc, char *argv[]) {
  system("clear");
  int opcoes;
  
  while (true) {
    setbuf(stdin,NULL);
    printf("\n=----------------OPÇÕES----------------=");
    printf(HWHT"\n[1] - Importar .txt com artigos para a tabela de registros"reset);
    printf(HWHT"\n[2] - Importar tabela de registros para a arvore"reset);
    printf(HGRN"\n[3] - Inserir um unico artigo na arvore"reset);
    printf(HWHT"\n[4] - Imprimir Arvore"reset);
    printf(HWHT"\n[5] - Procurar artigo por ID"reset);
    printf(HYEL"\n[6] - Excluir artigo por ID\n"reset);
    printf(RED"[7] - Sair do programa\n"reset);
    printf("=--------------------------------------=\n");
    printf(HMAG"Digite a opção [1-7] -> "reset);
    scanf("%d", &opcoes);
  
    switch (opcoes) {
      case 1: {
        char caminho[TAM];
        printf(HMAG"\nInforme o caminho do um arquivo ou 1 para usar o padrão: "reset);
        scanf("%s", caminho);
        if (strcmp(caminho, "1") != 0){
          criarTabelaRegistros(caminho);
        }
        else {
          criarTabelaRegistros(TXT_FILE);
        }
        break;
      }
  
      case 2:
        importarTabela();
        break;
      
      case 4:
        printf(HGRN"\n[*] Páginas Folhas  \n"reset);
        printf(HYEL"[*] Páginas Internas\n\n"reset);
        BPTreeImprimeArvoreCompleta();
        break;

      case 5: {
        int id, pos;
        printf(HMAG"\nInforme o ID do registro: "reset);
        scanf("%d", &id);
        imprimirArtigo(id);
        break;
      }
        
      case 6:
        // excluir artigo
      
      case 7:
        return EXIT_SUCCESS;
      
      default:
        continue;
    }
  }
  
  EXIT_SUCCESS;
}