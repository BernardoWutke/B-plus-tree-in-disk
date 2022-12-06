#include "../headers/util.h"
#include "../headers/pagina.h"
#include "../headers/b_plus_tree.h"

void fillLine(int size, char c){
    while((size--) > 0) printf("%c", c);
}

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

void imprimirPaciente(int id, int indexPagina){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");

    Pagina pag;
    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * indexPagina), SEEK_SET);
    fread(&pag, sizeof(Pagina), 1, arquivoArvore);

    int posicaoPaciente;
    for(int i = 0; i < pag.qtdElementos; ++i){
        if(pag.chave[i] == id){
            posicaoPaciente = pag.filho[i];
        }
    }

    fclose(arquivoArvore);
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb");

    Paciente paciente;
    fseek(arquivoRegistros, sizeof(Paciente) * posicaoPaciente, SEEK_SET);
    fread(&paciente, sizeof(Paciente), 1, arquivoRegistros);

    printf("-------------------------------------------------------\n");
    printf("|             F I C H A   P A C I E N T E             |\n");
    printf("-------------------------------------------------------\n");
    printf("| ID...........................................%06d |\n", paciente.id);
    printf("| Ano de nascimento..............................%04d |\n", paciente.anoNascimento);
    
    printf("| Nome");
    fillLine(47-strlen(paciente.nome), '.');
    printf("%s |\n", paciente.nome);

    printf("| Endereço");
    fillLine(43-strlen(paciente.endereco), '.');
    printf("%s |\n", paciente.endereco);

    printf("| Nome da mãe");
    fillLine(40-strlen(paciente.nomeMae), '.');
    printf("%s |\n", paciente.nomeMae);

    printf("| Nome do pai");
    fillLine(40-strlen(paciente.nomePai), '.');
    printf("%s |\n", paciente.nomePai);

    printf("| CPF");
    fillLine(48-strlen(paciente.CPF), '.');
    printf("%s |\n", paciente.CPF);
    printf("------------------------------------------------------\n\n");
}
