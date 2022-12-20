#include"../headers/b_plus_tree.h"
#include"../headers/pagina.h"
#include"../headers/util.h"

void addPagina(Pagina pag,int index);


void inicializarBP(){

    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");

    if (arquivoArvore == NULL) {
        BP_Tree arvore;
        arvore.ordem = ORDEM;
        arvore.qtdPaginas = 0;
        arvore.raiz = -1;
        arvore.qtdPacientes = 0;

        arquivoArvore = fopen(ARQUIVO_ARVORE, "wb+");
        fwrite(&arvore, sizeof(BP_Tree), 1, arquivoArvore);
    }
    
    fclose(arquivoArvore);
}

void imprimirPagina(Pagina pag, int isRoot){
    printf("------------------------------------------------------\n");
    if(isRoot)
        printf("|          P A G I N A    %03d    # R A I Z #         |\n",  pag.index);
    else
        printf("|                 P A G I N A    %03d                 |\n",  pag.index);
    printf("------------------------------------------------------\n");
    
    printf("| Tipo.");
    fillLine((pag.tipo == FOLHA ? 40 : 38), '.');
    printf("%s |\n", (pag.tipo == FOLHA ? "FOLHA" : "INTERNA"));

    if(!isRoot){
        printf("| Index pai.");
        fillLine(37, '.');
        printf("%03d |\n", pag.pai);
    }
    
    printf("| Qtd Elementos.");
    fillLine(33, '.');
    printf("%03d |\n", pag.qtdElementos);

    printf("| Chaves.");
    fillLine(44-(4*pag.qtdElementos), '.');
    for(int i = 0; i < pag.qtdElementos; ++i){
        printf("%03d ", pag.chave[i]);
    }
    printf("|\n");
    printf("| %s", (pag.tipo == FOLHA ? "Index registros." : "Filhos."));
    fillLine((pag.tipo == FOLHA ? 35 - 4*pag.qtdElementos : 44 - 4*(pag.qtdElementos + 1)), '.');
    for(int i = 0; i < pag.qtdElementos; ++i){
        printf("%03d ", pag.filho[i]);
    }
    if(pag.tipo == INTERNA)
        printf("%03d ", pag.filho[pag.qtdElementos]);
    printf("|\n");
    printf("------------------------------------------------------\n\n");
}

void imprimirArvore(){
  FILE *fp = fopen(ARQUIVO_ARVORE, "rb");


  if (fp == NULL) {
    perror("Não foi possível abrir o arquivo !!!");
    EXIT_FAILURE;
  }
  
  BP_Tree bp;
  fread(&bp, sizeof(BP_Tree), 1, fp);

    printf("------------------------------------------------------\n");
    printf("|                    A R V O R E                     |\n");
    printf("------------------------------------------------------\n");

    printf("| Ordem.");
    fillLine(41, '.');
    printf("%03d | \n", bp.ordem);

    printf("| Raiz.");
    fillLine(42, '.');
    printf("%03d |\n", bp.raiz);

    printf("| Quantidade de paginas.");
    fillLine(25, '.');
    printf("%03d |\n", bp.qtdPaginas);

    printf("| Quantidade de pacientes.");
    fillLine(23, '.');
    printf("%03d |\n", bp.qtdPacientes);
    printf("------------------------------------------------------\n");
    Pagina pag;
    //imprimir paginas
    int contadorPagina = sizeof(BP_Tree);
    while(fread(&pag, sizeof(Pagina), 1, fp)){
        contadorPagina += sizeof(Pagina);
        fseek(fp, contadorPagina, SEEK_SET);
        imprimirPagina(pag, pag.pai == -1);
    }
    fclose(fp);
}

int buscarPaginaLivre(){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, sizeof(BP_Tree), SEEK_SET);
    Pagina p;
    int k = 0;
    while(fread(&p, sizeof(Pagina), 1, arquivoArvore)) {
        if(p.foiDeletada) break;
        k++;
    }
    fclose(arquivoArvore);
    return k;
}

int buscarRegistroLivre(){
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");
    fseek(arquivoRegistros, 0, SEEK_SET);
    Paciente p;
    int k = 0;
    while(fread(&p, sizeof(Paciente), 1, arquivoRegistros)) {
        if(p.foiDeletado) break;
        k++;
    }
    fclose(arquivoRegistros);
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
        else {
            int i;
            for(i = 0; i < pag.qtdElementos; ++i){
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
            if(i == pag.qtdElementos){
                fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.filho[pag.qtdElementos]), SEEK_SET);
                fread(&pag, sizeof(Pagina), 1, arquivoArvore);
            }
        }
    }
}

void inserirNaPagina(Pagina* pagina, int chave, int regIndex) {
    pagina->chave[pagina->qtdElementos] = chave;
    pagina->filho[pagina->qtdElementos] = regIndex;
    pagina->qtdElementos++;
}

void ordenarInterna(Pagina *pagina) {
  int j, temp;
  FILE *arquivoArvore =  fopen(ARQUIVO_ARVORE, "rb+");

  
  for (int i = 1; i < pagina->qtdElementos; i++) {
    temp = pagina->chave[i];
    for (j = i; j > 0 && temp < pagina->chave[j - 1]; j--) {
      pagina->chave[j] = pagina->chave[j - 1];
    }
    pagina->chave[j] = temp;
  }
  
  temp = 0;
  
  Pagina filhoI, filhoJ;
  
  for(int i = 0; i < pagina->qtdElementos + 1; i++){
    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pagina->filho[i]), SEEK_SET);
    fread(&filhoI, sizeof(Pagina), 1, arquivoArvore);
  }

  for (int i = 0; i < pagina->qtdElementos; i++) {
    //Ler o filho I
    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pagina->filho[i]), SEEK_SET);
    fread(&filhoI, sizeof(Pagina), 1, arquivoArvore);
    for (j = i + 1; j < pagina->qtdElementos + 1; j++) {   
      //Ler o filho J
      fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pagina->filho[j]), SEEK_SET);
      fread(&filhoJ, sizeof(Pagina), 1, arquivoArvore);
      //Se a primira chave do filho i fo menor do que a primeira do filho j
      //troca os filhos de lugar
      if (filhoJ.chave[0] < filhoI.chave[0]) {
        temp = filhoI.index;
        pagina->filho[i] = filhoJ.index;
        pagina->filho[j] = temp;
      }
    }
  }
}


void fixOverflow(BP_Tree *bp_tree, Pagina* pagina){
    Pagina novaPagina;
    int indexNovaPagina = buscarPaginaLivre();
    inicializarPagina(&novaPagina, bp_tree->ordem, indexNovaPagina, pagina->tipo);

    int meio = pagina->qtdElementos >> 1;

    novaPagina.qtdElementos = meio+1;

    //linkando as paginas folhas
    novaPagina.indexProximaPagina = pagina->indexProximaPagina;
    pagina->indexProximaPagina = novaPagina.index;

    //fazendo copia dos elementos para a nova pagina

    for(int i = meio, k = 0; i <= pagina->qtdElementos; i++, k++){
        novaPagina.chave[k] = pagina->chave[i];
        novaPagina.filho[k] = pagina->filho[i];
        if(pagina->tipo == INTERNA){
            FILE *arquivoAvore = fopen(ARQUIVO_ARVORE, "rb+");
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
        ordenarInterna(&raiz);
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
        ordenarInterna(&pai);
        
        
        // verificar se a pagina pai deu overflow
        if(pai.qtdElementos > pai.ordem){
            // se deu, chama a função de fix dnv
            fixOverflow(bp_tree, &pai);
            addPagina(pai, pai.index); 
            ordenarInterna(&pai); 
            

        }
        

    }
    
}

void inserirPaciente(Paciente paciente){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");
    if(arquivoRegistros == NULL) 
        arquivoRegistros = fopen(ARQUIVO_REGISTROS , "wb+");

    BP_Tree bp_tree;
    Pagina pag;

    fseek(arquivoArvore, 0, SEEK_SET);
    fread(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);
    
    int indexRegistro = buscarRegistroLivre(); // precisa mudar

    bp_tree.qtdPacientes++;
    paciente.foiDeletado = 0;
    //verificar se já possui alguma pagina na arvore, se não, precisa criar a primeira
    if(bp_tree.raiz == -1){
        inicializarPagina(&pag, bp_tree.ordem, 0, FOLHA);

        pag.ordem = bp_tree.ordem;

        inserirNaPagina(&pag, paciente.id, 0);

        
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
        buscarPaciente(paciente.id, &indexPagina);

        fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * indexPagina), SEEK_SET);
        fread(&pag, sizeof(Pagina), 1, arquivoArvore);
   
        inserirNaPagina(&pag, paciente.id, indexRegistro);

        ordenarPaginaFolha(&pag);


        if(pag.qtdElementos > pag.ordem){
            fixOverflow(&bp_tree, &pag);
        }



        fseek(arquivoRegistros, sizeof(Paciente)*indexRegistro, SEEK_SET);
        fwrite(&paciente, sizeof(Paciente), 1, arquivoRegistros);

        fseek(arquivoArvore, 0, SEEK_SET);
        fwrite(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

        fseek(arquivoArvore, sizeof(BP_Tree)  + (sizeof(Pagina) * pag.index), SEEK_SET);
        fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);   

    }

    printf("\nPaciente cadastrado com sucesso!\n");
    printf("ID do paciente -> %d.\n\n", paciente.id);

    fclose(arquivoRegistros);
    fclose(arquivoArvore);
}

void addPagina(Pagina pag,int index){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * index), SEEK_SET);
    fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);
    fclose(arquivoArvore);
}

void deletarRegistros(int id){
    Paciente paciente;
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");
    fseek(arquivoRegistros, id*sizeof(Paciente), SEEK_SET);
    fread(&paciente, sizeof(Paciente), 1, arquivoRegistros);
    paciente.foiDeletado = 1;
    fseek(arquivoRegistros, -sizeof(Paciente), SEEK_CUR);
    fwrite(&paciente, sizeof(Paciente), 1, arquivoRegistros);
    fclose(arquivoRegistros);
}


void deletarPaciente(int id) {
    deletarRegistros(id);

    int indexPagina;
    buscarPaciente(id, &indexPagina);
    
    Pagina pagina;
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, sizeof(BP_Tree) + indexPagina*sizeof(Pagina), SEEK_SET);
    fread(&pagina, sizeof(Pagina), 1, arquivoArvore);

	int del = 0; 
    while(pagina.chave[del] != id) del++;

    
	for (int i = del; i < pagina.qtdElementos - 1; i++) {
		pagina.chave[i] = pagina.chave[i + 1];
		pagina.filho[i] = pagina.filho[i + 1];
	}
	pagina.qtdElementos--;

    BP_Tree bp_tree;

    fseek(arquivoArvore, 0, SEEK_SET);
    fread(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);
    bp_tree.qtdPacientes--;
    fseek(arquivoArvore, -sizeof(BP_Tree), SEEK_CUR);
    fwrite(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);
    fseek(arquivoArvore,pagina.index*sizeof(Paciente), SEEK_CUR);
    fwrite(&pagina, sizeof(Pagina), 1, arquivoArvore);

	// if (pagina->tipo == INTERNA) { // make links on leaves
	// 	Pagina* firstChild = (Pagina*)(pagina->filho[0]);
	// 	if (firstChild->tipo == FOLHA) { // which means delChild is also a leaf
	// 		Pagina* temp = (Pagina*)delChild;
	// 		int prevChild = temp->indexPaginaAnterior;
	// 		int succChild = temp->indexProximaPagina;
	// 		if (prevChild != NULL) prevChild->next = succChild;
	// 		if (succChild != NULL) succChild->last = prevChild;
	// 	}
	// }
	// if (del == 0 && !pagina->isRoot) { // some fathers' id should be changed
	// 	Pagina* temp = pagina;
	// 	while (!temp->isRoot && temp == temp->father->filho[0]) {
	// 		temp->father->chave[0] = pagina->chave[0];
	// 		temp = temp->father;
	// 	}
	// 	if (!temp->isRoot) {
	// 		temp = temp->father;
	// 		int i = Binary_Search(temp, id);
	// 		temp->chave[i] = pagina->chave[0];
	// 	}
	// }
	// free(delChild);
	// if (pagina->qtdElementos * 2 < MaxChildNumber)
	// 	Redistribute(pagina);
    fclose(arquivoArvore);
}

void  imprimirIntervaloPacientes(int initialID,int finalID){
    int indexPagina;
    buscarPaciente(initialID,&indexPagina);
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");
    
    Pagina pagina;
    fseek(arquivoArvore, sizeof(BP_Tree) + indexPagina*sizeof(Pagina), SEEK_SET);
    fread(&pagina, sizeof(Pagina), 1, arquivoArvore);
    //buscar elemento inicial na pagina
    int i = 0;
    while(pagina.chave[i] < initialID) {
        i++;
        if(i >= pagina.qtdElementos && pagina.indexProximaPagina != -1){
            //buscar proxima pagina
            fseek(arquivoArvore, sizeof(BP_Tree) + pagina.filho[i]*sizeof(Pagina), SEEK_SET);
            fread(&pagina, sizeof(Pagina), 1, arquivoArvore);
            i = 0;
        }
    }
    //imprimir elementos da pagina 
    while( pagina.chave[i] <= finalID ){
        imprimirPaciente(pagina.chave[i], pagina.index);
        i++;
        if(i >= pagina.qtdElementos && pagina.indexProximaPagina != -1){
            //buscar proxima pagina
            fseek(arquivoArvore, sizeof(BP_Tree) + pagina.indexProximaPagina*sizeof(Pagina), SEEK_SET);
            fread(&pagina, sizeof(Pagina), 1, arquivoArvore);
            i = 0;
        }
        else if(i >= pagina.qtdElementos && pagina.indexProximaPagina == -1)
            break;
    }
    fclose(arquivoArvore);
    fclose(arquivoRegistros);   
}
