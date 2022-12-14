#include"../headers/b_plus_tree.h"
#include"../headers/pagina.h"
    
void imprimirArvoreHeaderReferencia(BP_Tree *bp){
    printf("Ordem: %d \n", bp->ordem);
    printf("Quantidade: %d \n", bp->qtdPaginas);
    printf("Raiz: %d \n", bp->raiz);
}

void inicializarBP(){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");

    if (arquivoArvore == NULL) {
        BP_Tree arvore;
        arvore.ordem = ORDEM;
        arvore.qtdPaginas = 0;
        arvore.raiz = -1;
        arvore.proximoID = 0;

        arquivoArvore = fopen(ARQUIVO_ARVORE, "wb+");
        fwrite(&arvore, sizeof(BP_Tree), 1, arquivoArvore);
    }
    
    fclose(arquivoArvore);
}

void imprimirPagina(Pagina pag){
    printf("Tipo: %d \n", pag.tipo);
    printf("Index: %d \n", pag.index);
    printf("Qtd Elementos: %d \n", pag.qtdElementos);
    printf("Chaves: ");
    for(int i = 0; i < pag.qtdElementos; ++i){
        printf("%d ", pag.chave[i]);
    }
    printf("\n");
    printf("Filhos: ");
    for(int i = 0; i < pag.qtdElementos + 1; ++i){
        printf("%d ", pag.filho[i]);
    }
    printf("\n");
}

void imprimirArvoreHeader(){
  printf("===============================\n");
  FILE *fp = fopen(ARQUIVO_ARVORE, "rb");


  if (fp == NULL) {
    perror("Não foi possível abrir o arquivo !!!");
    EXIT_FAILURE;
  }
  
  BP_Tree bp;
  fread(&bp, sizeof(BP_Tree), 1, fp);
  printf("Ordem: %d \n", bp.ordem);
  printf("Quantidade: %d \n", bp.qtdPaginas);
  printf("Raiz: %d \n", bp.raiz);
  printf("proximo ID: %d\n", bp.proximoID);
  printf("===============================\n");
  Pagina pag;
  //imprimir paginas
  while(fread(&pag, sizeof(Pagina), 1, fp)){
    imprimirPagina(pag);
  }
  printf("===============================\n");
  fclose(fp);
}

int buscarPaciente(int id, int *indexPagina){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");
    
    if(arquivoArvore == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return false;
    }

    BP_Tree bp_tree;
    fread(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

    //verificar se a arvore possui raiz
    if(bp_tree.raiz == -1){
        *indexPagina = -1;
        fclose(arquivoArvore);
        return false;
    }

    Pagina pag;
    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * bp_tree.raiz), SEEK_SET);
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

void inserirPaciente(Paciente paciente){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");
    if(arquivoRegistros == NULL) 
    arquivoRegistros = fopen(ARQUIVO_REGISTROS , "wb+");

    BP_Tree bp_tree;
    fseek(arquivoArvore, 0, SEEK_SET);
    fread(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);
    Pagina pag;
    
    /*
        Primeiro precisa definir o novo ID.
        Sugestão: Criar um campo no header chamado nextID que salva o valor do próximo ID
        Basicamente este campo vai ser a quantidade de pacientes cadastrados. O i-ésimo paciente
        tem o ID i.
    */
    int id = bp_tree.proximoID++; // mudar isso aqui para calcular o id corretamente;


    //verificar se já possui alguma pagina na arvore, se não, precisa criar a primeira
    if(bp_tree.raiz == -1){
        inicializarPagina(&pag, bp_tree.ordem, FOLHA);
        pag.index = 0;
        pag.ordem = bp_tree.ordem;
        pag.chave[0] = 0;  
        pag.filho[0] = 0;
        pag.qtdElementos = 1;
        paciente.id = 0;
        
        fwrite(&paciente, sizeof(Paciente), 1, arquivoRegistros);

        fseek(arquivoArvore, 0, SEEK_SET);
        bp_tree.qtdPaginas++;
        bp_tree.raiz = 0;
        fwrite(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);
        fseek(arquivoArvore, sizeof(BP_Tree), SEEK_SET);
        fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);
    }
    else{
        int indexPagina;
        buscarPaciente(id, &indexPagina);

        fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * indexPagina), SEEK_SET);
        fread(&pag, sizeof(Pagina), 1, arquivoArvore);
   
        pag.chave[pag.qtdElementos] = id;
        pag.filho[pag.qtdElementos] = id;
        pag.qtdElementos++;
        
        paciente.id = id;

        fseek(arquivoRegistros, sizeof(Paciente)*id, SEEK_SET);
        fwrite(&paciente, sizeof(Paciente), 1, arquivoRegistros);

        fseek(arquivoArvore, 0, SEEK_SET);
        fwrite(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

        fseek(arquivoArvore, sizeof(BP_Tree)  + (sizeof(Pagina) * pag.index), SEEK_SET);
        fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);   

    }
    
    // Tratar o overflow em uma outra função
    fclose(arquivoRegistros);
    fclose(arquivoArvore);
}

void inserirEmFolha(Pagina* pagina, int chave, int regIndex) {
    pagina->chave[pagina->qtdElementos] = chave;
    pagina->filho[pagina->qtdElementos] = regIndex;
    pagina->qtdElementos++;
}
