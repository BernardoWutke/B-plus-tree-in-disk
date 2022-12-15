#include"../headers/b_plus_tree.h"
#include"../headers/pagina.h"

void addPagina(Pagina pag,int index);
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
    printf("Index pai: %d \n", pag.pai);
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
  Pagina pag;
  //imprimir paginas
  int contadorPagina = sizeof(BP_Tree);
  while(fread(&pag, sizeof(Pagina), 1, fp)){
    contadorPagina += sizeof(Pagina);
    fseek(fp, contadorPagina, SEEK_SET);
    printf("===============================\n");
    imprimirPagina(pag);
    printf("===============================\n");
  }
  fclose(fp);
}

int buscarPaginaLivre(int qtdPaginas){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, sizeof(BP_Tree), SEEK_SET);
    Pagina p;
    int k = 0;
    while(fread(&p, sizeof(Pagina), 1, arquivoArvore)) {
        if(p.foiDeletada) break;
        k++;
    }
    return k;
}

int buscarRegistroLivre(FILE *arquivoRegistros, int qtdRegistros){
    fseek(arquivoRegistros, sizeof(Paciente), SEEK_SET);
    Paciente p;
    fread(&p, sizeof(Paciente), 1, arquivoRegistros);
    int k = 0;
    while(!p.foiDeletado && k < qtdRegistros) {
        k++;
        fread(&p, sizeof(Paciente), 1, arquivoRegistros);
    }
    return k;
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

void inserirNaPagina(Pagina* pagina, int chave, int regIndex) {
    pagina->chave[pagina->qtdElementos] = chave;
    pagina->filho[pagina->qtdElementos] = regIndex;
    pagina->qtdElementos++;
}

void fixOverflow(BP_Tree *bp_tree, Pagina* pagina){

    Pagina novaPagina;
    int indexNovaPagina = buscarPaginaLivre(bp_tree->qtdPaginas);
    inicializarPagina(&novaPagina, bp_tree->ordem, indexNovaPagina, pagina->tipo);

    int meio = pagina->qtdElementos >> 1;

    
    novaPagina.index = buscarPaginaLivre(bp_tree->qtdPaginas);
    novaPagina.qtdElementos = meio+1;

    //fazendo copia dos elementos para a nova pagina


    

    for(int i = meio, k = 0; i <= pagina->qtdElementos; i++, k++){
        novaPagina.chave[k] = pagina->chave[i];
        novaPagina.filho[k] = pagina->filho[i];
        if(pagina->tipo == INTERNA){
            FILE *arquivoAvore = fopen(ARQUIVO_ARVORE, "rb+");
            printf("CAIU NO LAÇO \n");
            Pagina tempPagina;
            fseek(arquivoAvore, sizeof(BP_Tree) + sizeof(Pagina)*pagina->filho[k], SEEK_SET);
            fread(&tempPagina, sizeof(Pagina), 1, arquivoAvore);
            tempPagina.pai = novaPagina.index;
            fclose(arquivoAvore);
        }
    }

    pagina->qtdElementos -= (meio+1);
    addPagina(novaPagina, novaPagina.index);
    bp_tree->qtdPaginas++;

    printf("#\n");

    if(pagina->index == bp_tree->raiz){
        
        Pagina raiz;
        int indexRaiz = buscarPaginaLivre(bp_tree->qtdPaginas);
        inicializarPagina(&raiz, bp_tree->ordem, indexRaiz, INTERNA);
        
        inserirNaPagina(&raiz, novaPagina.chave[0], pagina->index);
        raiz.filho[1] = novaPagina.index;

        novaPagina.pai = indexRaiz;
        pagina->pai = indexRaiz;
        addPagina(novaPagina, novaPagina.index);
        addPagina(raiz, raiz.index);

        bp_tree->raiz = indexRaiz;
        bp_tree->qtdPaginas++;

    }
    else{
        novaPagina.pai = pagina->pai;
        addPagina(novaPagina, novaPagina.index);

        // acessar a pagina pai da pagina atual
        FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
        Pagina pai;
        fseek(arquivoArvore, sizeof(BP_Tree) + sizeof(Pagina) * pagina->pai, SEEK_SET);
        fread(&pai, sizeof(Pagina), 1, arquivoArvore);

        // inserir nela o primeiro elemento da nova pagina
        inserirNaPagina(&pai, novaPagina.chave[0], pagina->index);
        pai.filho[pai.qtdElementos] = novaPagina.index;
        fclose(arquivoArvore);
        addPagina(pai, pai.index);
        
        // verificar se a pagina pai deu overflow
        if(pai.qtdElementos > pai.ordem){
            // se deu, chama a função de fix dnv
            fixOverflow(bp_tree, &pai);
            addPagina(pai, pai.index);    
        }
        printf("%d\n", pai.qtdElementos);
        
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
    
    int id = bp_tree.proximoID++; // precisa mudar


    //verificar se já possui alguma pagina na arvore, se não, precisa criar a primeira
    if(bp_tree.raiz == -1){
        inicializarPagina(&pag, bp_tree.ordem, 0, FOLHA);

        pag.ordem = bp_tree.ordem;

        inserirNaPagina(&pag, 0, 0);

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
   
        inserirNaPagina(&pag, id, id);
        
        paciente.id = id;

        if(pag.qtdElementos > pag.ordem){
            fixOverflow(&bp_tree, &pag);
        }



        fseek(arquivoRegistros, sizeof(Paciente)*id, SEEK_SET);
        fwrite(&paciente, sizeof(Paciente), 1, arquivoRegistros);

        fseek(arquivoArvore, 0, SEEK_SET);
        fwrite(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

        fseek(arquivoArvore, sizeof(BP_Tree)  + (sizeof(Pagina) * pag.index), SEEK_SET);
        fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);   

    }
    fclose(arquivoRegistros);
    fclose(arquivoArvore);
}

void addPagina(Pagina pag,int index){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * index), SEEK_SET);
    fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);
    fclose(arquivoArvore);
}
