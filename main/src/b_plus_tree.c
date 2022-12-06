#include"../headers/b_plus_tree.h"
#include"../headers/pagina.h"

    


void inicializarBP(BP_Tree *bp){
    // Tenta ler o arquivo base

    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");
    // Se já tiver arquivo, carrega o cabeçalho em bp
    if (arquivoArvore != NULL) {
        fseek(arquivoArvore, 0, SEEK_SET);
        fread(bp, sizeof(BP_Tree), 1, arquivoArvore);
        fclose(arquivoArvore);
    }
    else{ // Se não cria um novo arquivo com um cabeçalho do zero
        bp->qtde = 0;
        bp->raiz = -1;
        arquivoArvore = fopen(ARQUIVO_ARVORE, "wb+");
        fseek(arquivoArvore, 0, SEEK_SET);
        fwrite(bp, sizeof(BP_Tree), 1, arquivoArvore);
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

int buscarElemento(int id, int *indexPagina, BP_Tree bp_tree){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");
    
    if(arquivoArvore == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return false;
    }

    //verificar se a arvore possui raiz
    if(bp_tree.raiz == -1){
        *indexPagina = -1;
        fclose(arquivoArvore);
        return false;
    }

    Pagina pag;
    fseek(arquivoArvore, sizeof(Pagina) * bp_tree.raiz, SEEK_CUR);
    fread(&pag, sizeof(Pagina), 1, arquivoArvore);

    while(true){
        if(pag.tipo == FOLHA){
            *indexPagina = pag.index;
            for(int i = 0; i < pag.qtdElementos; ++i){
                if(pag.chave[i] == id){
                    fclose(arquivoArvore);
                    return true;
                }
            }

            fclose(arquivoArvore);
            return false;
        }
        // verificar se a chave buscada é menor que a chave na posição 0
        else if(id < pag.chave[0]){
            fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.filho[0]), SEEK_SET);
            fread(&pag, sizeof(Pagina), 1, arquivoArvore);
        }
        else {
            for(int i = 1; i < pag.qtdElementos; ++i){
                if(id < pag.chave[i]){
                    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.filho[i]), SEEK_SET);
                    fread(&pag, sizeof(Pagina), 1, arquivoArvore);
                    break;
                }
                else if(id == pag.chave[i]){
                    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.filho[i+1]), SEEK_SET);
                    fread(&pag, sizeof(Pagina), 1, arquivoArvore);
                    break;
                }
            }
            fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.filho[pag.qtdElementos]), SEEK_SET);
            fread(&pag, sizeof(Pagina), 1, arquivoArvore);
        }
    }
}

int inserirPaciente(Paciente paciente, BP_Tree bp_tree){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "ab+");
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "ab+");
    /*
        Primeiro precisa definir o novo ID.
        Sugestão: Criar um campo no header chamado nextID que salva o valor do próximo ID
        Basicamente este campo vai ser a quantidade de pacientes cadastrados. O i-ésimo paciente
        tem o ID i.
    */
    int id = 0; // mudar isso aqui para calcular o id corretamente;


    //verificar se já possui alguma pagina na arvore, se não, precisa criar a primeira
    if(bp_tree.raiz == -1){
        Pagina pag = criarPagina(bp_tree.ordem, FOLHA);
        pag.chave[0] = id;
        pag.filho[0] = 0;

        fseek(arquivoRegistros, 0, SEEK_SET);
        printf("entrou\n");
        fwrite(&paciente, sizeof(Paciente), 1, arquivoRegistros);

        fseek(arquivoArvore, sizeof(BP_Tree), SEEK_SET);
        fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);

        fclose(arquivoArvore);
        fclose(arquivoRegistros);
    }
    /*
        Terceiro precisa chamar a função de busca para encontrar a posição e a pagina em que o novo paciente
        será cadastrado

        Inserir o paciente na pagina e ordenar ela

        ## Tratar o overflow em uma outra função ##
    */
}
