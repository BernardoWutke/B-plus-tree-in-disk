 #include "../headers/b_plus_tree.h"
#include "../headers/pagina.h"
#include "../headers/util.h"
//resolver mais tarde
int controleFixUnderflow = 0;

void addPagina(Pagina pag, int index);
void concatenarExterna(Pagina *paginaAtual, Pagina *paginaIrma, Pagina *paginaPai, BP_Tree* bp_tree);
void concatenarInterna(Pagina *paginaAtual, Pagina *paginaIrma, Pagina *paginaPai, BP_Tree* bp_tree);
void atualizarPaginaInterna(int indexPai, int idRemovido, int novoId);
void fixUnderflow(Pagina *paginaAtual);
void inicializarBP()
{

    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");

    if (arquivoArvore == NULL)
    {
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

void imprimirPagina(Pagina pag, int isRoot)
{
    printf("------------------------------------------------------\n");
    if (isRoot)
        printf("|          P A G I N A    %03d    # R A I Z #         |\n", pag.index);
    else
        printf("|                 P A G I N A    %03d                 |\n", pag.index);
    printf("------------------------------------------------------\n");

    printf("| Tipo.");
    fillLine((pag.tipo == FOLHA ? 40 : 38), '.');
    printf("%s |\n", (pag.tipo == FOLHA ? "FOLHA" : "INTERNA"));

    if (!isRoot)
    {
        printf("| Index pai.");
        fillLine(37, '.');
        printf("%03d |\n", pag.pai);
    }

    printf("| Qtd Elementos.");
    fillLine(33, '.');
    printf("%03d |\n", pag.qtdElementos);

    printf("| Chaves.");
    fillLine(44 - (4 * pag.qtdElementos), '.');
    for (int i = 0; i < pag.qtdElementos; ++i)
    {
        printf("%03d ", pag.chave[i]);
    }
    printf("|\n");
    printf("| %s", (pag.tipo == FOLHA ? "Index registros." : "Filhos."));
    fillLine((pag.tipo == FOLHA ? 35 - 4 * pag.qtdElementos : 44 - 4 * (pag.qtdElementos + 1)), '.');
    for (int i = 0; i < pag.qtdElementos; ++i)
    {
        printf("%03d ", pag.filho[i]);
    }
    if (pag.tipo == INTERNA)
        printf("%03d ", pag.filho[pag.qtdElementos]);
    printf("|\n");
    printf("------------------------------------------------------\n\n");
}

void imprimirArvore()
{
    FILE *fp = fopen(ARQUIVO_ARVORE, "rb");

    if (fp == NULL)
    {
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
    if(bp.raiz == -1){
        fillLine(43, '.');
        printf("-1 |\n");
    }
    else{
        fillLine(42, '.');
        printf("%03d |\n", bp.raiz);
    }

    printf("| Quantidade de paginas.");
    fillLine(25, '.');
    printf("%03d |\n", bp.qtdPaginas);

    printf("| Quantidade de pacientes.");
    fillLine(23, '.');
    printf("%03d |\n", bp.qtdPacientes);
    printf("------------------------------------------------------\n");
    Pagina pag;
    // imprimir paginas
    int contadorPagina = sizeof(BP_Tree);
    while (fread(&pag, sizeof(Pagina), 1, fp))
    {
        contadorPagina += sizeof(Pagina);
        fseek(fp, contadorPagina, SEEK_SET);
        if(!pag.foiDeletada)
            imprimirPagina(pag, pag.pai == -1);
    }
    fclose(fp);
}

int buscarPaginaLivre()
{
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, sizeof(BP_Tree), SEEK_SET);
    Pagina p;
    int k = 0;
    while (fread(&p, sizeof(Pagina), 1, arquivoArvore))
    {
        if (p.foiDeletada)
            break;
        k++;
    }
    fclose(arquivoArvore);
    return k;
}

int buscarRegistroLivre()
{
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");
    fseek(arquivoRegistros, 0, SEEK_SET);
    Paciente p;
    int k = 0;
    while (fread(&p, sizeof(Paciente), 1, arquivoRegistros))
    {
        if (p.foiDeletado)
            break;
        k++;
    }
    fclose(arquivoRegistros);
    return k;
}

int buscarPaciente(int id, int *indexPagina)
{
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");

    if (arquivoArvore == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return false;
    }

    BP_Tree bp_tree;
    fread(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

    // verificar se a arvore possui raiz
    if (bp_tree.raiz == -1)
    {
        *indexPagina = -1;
        fclose(arquivoArvore);
        return false;
    }

    Pagina pag;
    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * bp_tree.raiz), SEEK_SET);
    fread(&pag, sizeof(Pagina), 1, arquivoArvore);

    while (true)
    {
        if (pag.tipo == FOLHA)
        {
            *indexPagina = pag.index;
            for (int i = 0; i < pag.qtdElementos; ++i)
            {
                if (pag.chave[i] == id)
                {
                    fclose(arquivoArvore);
                    return true;
                }
            }

            fclose(arquivoArvore);
            return false;
        }
        else
        {
            int i;
            for (i = 0; i < pag.qtdElementos; ++i)
            {
                if (id < pag.chave[i])
                {
                    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.filho[i]), SEEK_SET);
                    fread(&pag, sizeof(Pagina), 1, arquivoArvore);
                    break;
                }
                else if (id == pag.chave[i])
                {
                    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.filho[i + 1]), SEEK_SET);
                    fread(&pag, sizeof(Pagina), 1, arquivoArvore);
                    break;
                }
            }
            if (i == pag.qtdElementos)
            {
                fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.filho[pag.qtdElementos]), SEEK_SET);
                fread(&pag, sizeof(Pagina), 1, arquivoArvore);
            }
        }
    }
}

void inserirNaPagina(Pagina *pagina, int chave, int regIndex)
{
    pagina->chave[pagina->qtdElementos] = chave;
    pagina->filho[pagina->qtdElementos] = regIndex;
    pagina->qtdElementos++;
}

void ordenarInterna(Pagina *pagina)
{
    int j, temp;
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");

    for (int i = 1; i < pagina->qtdElementos; i++)
    {
        temp = pagina->chave[i];
        for (j = i; j > 0 && temp < pagina->chave[j - 1]; j--)
        {
            pagina->chave[j] = pagina->chave[j - 1];
        }
        pagina->chave[j] = temp;
    }

    temp = 0;

    Pagina filhoI, filhoJ;

    for (int i = 0; i < pagina->qtdElementos + 1; i++)
    {
        fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pagina->filho[i]), SEEK_SET);
        fread(&filhoI, sizeof(Pagina), 1, arquivoArvore);
    }

    for (int i = 0; i < pagina->qtdElementos; i++)
    {
        // Ler o filho I
        fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pagina->filho[i]), SEEK_SET);
        fread(&filhoI, sizeof(Pagina), 1, arquivoArvore);
        for (j = i + 1; j < pagina->qtdElementos + 1; j++)
        {
            // Ler o filho J
            fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pagina->filho[j]), SEEK_SET);
            fread(&filhoJ, sizeof(Pagina), 1, arquivoArvore);
            // Se a primira chave do filho i fo menor do que a primeira do filho j
            // troca os filhos de lugar
            if (filhoJ.chave[0] < filhoI.chave[0])
            {
                temp = filhoI.index;
                pagina->filho[i] = filhoJ.index;
                pagina->filho[j] = temp;
            }
        }
    }
}

void fixOverflow(BP_Tree *bp_tree, Pagina *pagina)
{
    Pagina novaPagina;
    int indexNovaPagina = buscarPaginaLivre();
    inicializarPagina(&novaPagina, bp_tree->ordem, indexNovaPagina, pagina->tipo);

    int meio = pagina->qtdElementos >> 1;

    novaPagina.qtdElementos = meio + 1;

    // linkando as paginas folhas
    novaPagina.indexProximaPagina = pagina->indexProximaPagina;
    pagina->indexProximaPagina = novaPagina.index;

    // fazendo copia dos elementos para a nova pagina

    for (int i = meio, k = 0; i <= pagina->qtdElementos; i++, k++)
    {
        novaPagina.chave[k] = pagina->chave[i];
        novaPagina.filho[k] = pagina->filho[i];
    }

    pagina->qtdElementos -= (meio + 1);
    addPagina(novaPagina, novaPagina.index);

    bp_tree->qtdPaginas++;

    if (pagina->index == bp_tree->raiz)
    {

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
        addPagina(*pagina, pagina->index);
        
    }
    else
    {
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
        addPagina(*pagina, pagina->index);
        
        ordenarInterna(&pai);
        
        
        // verificar se a pagina pai deu overflow
        if (pai.qtdElementos > pai.ordem)
        {
            // se deu, chama a função de fix dnv
            fixOverflow(bp_tree, &pai);
            ordenarInterna(&pai);
            addPagina(pai, pai.index);
        }
    }
    if(novaPagina.tipo == INTERNA){
        for(int i = 0; i < novaPagina.qtdElementos-1; i++){
            novaPagina.chave[i] = novaPagina.chave[i+1];
            novaPagina.filho[i] = novaPagina.filho[i+1];
        }
        novaPagina.qtdElementos--;
        novaPagina.filho[novaPagina.qtdElementos] = novaPagina.filho[novaPagina.qtdElementos+1];
        addPagina(novaPagina, novaPagina.index);
        
        for(int k = 0; k <= novaPagina.qtdElementos; k++){
            FILE *arquivoAvore = fopen(ARQUIVO_ARVORE, "rb+");
            Pagina tempPagina;

            fseek(arquivoAvore, sizeof(BP_Tree) + sizeof(Pagina) * novaPagina.filho[k], SEEK_SET);
            fread(&tempPagina, sizeof(Pagina), 1, arquivoAvore);
            tempPagina.pai = novaPagina.index;
            fseek(arquivoAvore, sizeof(BP_Tree) + sizeof(Pagina) * novaPagina.filho[k], SEEK_SET);
            fwrite(&tempPagina, sizeof(Pagina), 1, arquivoAvore);
            
            fclose(arquivoAvore);
        }
    }
    
}

void inserirPaciente(Paciente paciente)
{
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");
    if (arquivoRegistros == NULL)
        arquivoRegistros = fopen(ARQUIVO_REGISTROS, "wb+");

    BP_Tree bp_tree;
    Pagina pag;

    fseek(arquivoArvore, 0, SEEK_SET);
    fread(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

    int indexRegistro = buscarRegistroLivre(); // precisa mudar

    bp_tree.qtdPacientes++;
    paciente.foiDeletado = 0;
    // verificar se já possui alguma pagina na arvore, se não, precisa criar a primeira
    if (bp_tree.raiz == -1)
    {
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
    else
    {
        int indexPagina;
        buscarPaciente(paciente.id, &indexPagina);

        fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * indexPagina), SEEK_SET);
        fread(&pag, sizeof(Pagina), 1, arquivoArvore);

        inserirNaPagina(&pag, paciente.id, indexRegistro);

        ordenarPaginaFolha(&pag);

        if (pag.qtdElementos > pag.ordem)
        {
            fixOverflow(&bp_tree, &pag);
        } else {
            fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * pag.index), SEEK_SET);
            fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);
        }

        fseek(arquivoRegistros, sizeof(Paciente) * indexRegistro, SEEK_SET);
        fwrite(&paciente, sizeof(Paciente), 1, arquivoRegistros);

        fseek(arquivoArvore, 0, SEEK_SET);
        fwrite(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

        
    }

    printf("\nPaciente cadastrado com sucesso!\n");
    printf("ID do paciente -> %d.\n\n", paciente.id);

    fclose(arquivoRegistros);
    fclose(arquivoArvore);
}

void addPagina(Pagina pag, int index)
{
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, sizeof(BP_Tree) + (sizeof(Pagina) * index), SEEK_SET);
    fwrite(&pag, sizeof(Pagina), 1, arquivoArvore);
    fclose(arquivoArvore);
}

void deletarRegistros(int indexRegistro)
{
    Paciente paciente;
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");
    fseek(arquivoRegistros, indexRegistro * sizeof(Paciente), SEEK_SET);
    fread(&paciente, sizeof(Paciente), 1, arquivoRegistros);
    paciente.foiDeletado = 1;
    fseek(arquivoRegistros, -sizeof(Paciente), SEEK_CUR);
    fwrite(&paciente, sizeof(Paciente), 1, arquivoRegistros);
    fclose(arquivoRegistros);
}

void redistribute(Pagina* paginaAtual, Pagina* pai, int posFilho, Pagina* paginaIrma, int posIrma)
{   
    printf("proxima pagina %d\n", paginaAtual->indexProximaPagina);
    if(paginaAtual->tipo == FOLHA){
        if(posIrma == paginaAtual->indexProximaPagina){
            printf("entrou com irmão a direita.\n");
            //mexe na pagina atual
            paginaAtual->chave[paginaAtual->qtdElementos] = paginaIrma->chave[0];
            paginaAtual->filho[paginaAtual->qtdElementos] = paginaIrma->filho[0];
            paginaAtual->qtdElementos++;
            //mexendo no pai, fazendo a trola
            pai->chave[posFilho] = paginaIrma->chave[1];
            //mexendo na pagina irma removendo o primeiro elemento
            for(int i = 0; i < paginaIrma->qtdElementos-1; i++){
                paginaIrma->chave[i] = paginaIrma->chave[i+1];
                paginaIrma->filho[i] = paginaIrma->filho[i+1];
            }
            paginaIrma->qtdElementos--;
        } else{
            printf("entrou com irmão a esquerda.\n");
            //mover para frente todos os elementos
            for(int i = paginaAtual->qtdElementos; i > 0; i--){
                paginaAtual->chave[i] = paginaAtual->chave[i-1];
                paginaAtual->filho[i] = paginaAtual->filho[i-1];
            }
            //mexe na pagina atual
            paginaAtual->chave[0] = paginaIrma->chave[paginaIrma->qtdElementos-1];
            paginaAtual->filho[0] = paginaIrma->filho[paginaIrma->qtdElementos-1];
            paginaAtual->qtdElementos++;
            //mexendo no pai, fazendo a troca
            pai->chave[posFilho] = paginaIrma->chave[paginaIrma->qtdElementos-1];
            //mexendo na pagina irma removendo o ultimo elemento
            paginaIrma->qtdElementos--;
        }
    }

}

void swap(Pagina *p1, Pagina *p2)
{
    Pagina aux = *p1;
    *p1 = *p2;
    *p2 = aux;
}

void concatenarExterna(Pagina *paginaAtual, Pagina *paginaIrma, Pagina *paginaPai, BP_Tree* bp_tree) {
    /*Passando como paramento @p_um e @paginaIrma sozinha a função irá descobrir qual está desbalanceada
    e assim saberá da forma correta quem vai concatenar em quem*/
    
    //Encontrando a posicao da pagina atual em relacao a pagina pai
    int del = 0;
    while (paginaPai->filho[del] != paginaAtual->index)
        del++;

    //encontrando a posição da chave que referencia a pagina Atual
    int posPai = (paginaPai->qtdElementos == del) ? paginaPai->qtdElementos-1 : del;

    // verificar se a pagina atual está a esquerda da irma
    if (paginaAtual->indexProximaPagina == paginaIrma->index)
    {
        //achando a posição do filho da pagina pai que é a pagina atual
        int posAtual = 0;
        while (paginaPai->filho[posAtual] != paginaAtual->index)
            posAtual++;
        //vereficar se existe uma pagina anterior a pagina atual
        if(posAtual != 0){
            FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
            //pegar referencia da pagina anterior da pagina atual, referenciando a pagina irma
            fseek(arquivoArvore, sizeof(BP_Tree) + paginaPai->filho[posAtual-1] * sizeof(Pagina), SEEK_SET);
            Pagina paginaAnterior;
            fread(&paginaAnterior, sizeof(Pagina), 1, arquivoArvore);
            paginaAnterior.indexProximaPagina = paginaIrma->index;
            fseek(arquivoArvore, sizeof(BP_Tree) + paginaAnterior.index * sizeof(Pagina), SEEK_SET);
            fwrite(&paginaAnterior, sizeof(Pagina), 1, arquivoArvore);
            fclose(arquivoArvore);
        }
        paginaAtual->indexProximaPagina = paginaIrma->indexProximaPagina;
        swap(paginaAtual, paginaIrma);
    }
    // concatenar paginaIrma em paginaAtual
    for (int i = 0; i < paginaIrma->qtdElementos; i++)
    {
        paginaAtual->chave[paginaAtual->qtdElementos] = paginaIrma->chave[i];
        paginaAtual->filho[paginaAtual->qtdElementos] = paginaIrma->filho[i];
        paginaAtual->qtdElementos++;
    }

    printf("pos pai %d \n", posPai);
    //removendo referencia a pagina irma/atual
    for (int i = posPai; i < paginaPai->qtdElementos-1; i++)
    {
        paginaPai->chave[i] = paginaPai->chave[i + 1];
        paginaPai->filho[i] = paginaPai->filho[i + 1];
    }
    printf("ultima %d => ", paginaPai->filho[paginaPai->qtdElementos-1]);
    paginaPai->filho[paginaPai->qtdElementos-1] = paginaPai->filho[paginaPai->qtdElementos];
    printf("%d\n", paginaPai->filho[paginaPai->qtdElementos-1]);
    paginaPai->qtdElementos--;

    for(int i = 0; i <= paginaPai->qtdElementos; i++){
        printf("%d ", paginaPai->filho[i]);
    }
    printf("\n");

    if(del > 1){
        Pagina tempPagina;
        FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
        fseek(arquivoArvore, sizeof(BP_Tree) + paginaPai->filho[del-2] * sizeof(Pagina), SEEK_SET);
        fread(&tempPagina, sizeof(Pagina), 1, arquivoArvore);
        printf("del %d\n", del);
        printf("pagina Index %d || prox atual %d || novo Prox %d\n", tempPagina.index, tempPagina.indexProximaPagina, paginaAtual->index);
        tempPagina.indexProximaPagina = paginaAtual->index;
        fseek(arquivoArvore, sizeof(BP_Tree) +  paginaPai->filho[del-2] * sizeof(Pagina), SEEK_SET);
        fwrite(&tempPagina, sizeof(Pagina), 1, arquivoArvore);
        fclose(arquivoArvore);
    }
    
    //verificando se a pagina pai é a raiz, se for, atualiza a raiz
    if(paginaPai->pai == -1 && paginaPai->qtdElementos == 0){
        paginaPai->foiDeletada = 1;
        bp_tree->raiz = paginaAtual->index;
        bp_tree->qtdPaginas--;
        paginaAtual->pai = -1;
    }
    
    //indicando que a pagina irma foi deletada
    paginaIrma->foiDeletada = 1;
    bp_tree->qtdPaginas--;

    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, 0, SEEK_SET);
    fwrite(bp_tree, sizeof(BP_Tree), 1, arquivoArvore);
    fclose(arquivoArvore);    

    ordenarPaginaFolha(paginaAtual);
    printf("index pai %d || qtdElemento %d\n", paginaPai->index, paginaPai->qtdElementos);
    //verificando se a pagina pai está desbalanceada
    if( paginaPai->pai != -1 && (paginaPai->qtdElementos < (ORDEM + 1) /2 )){
        printf("underflow pro pai \n");
        fixUnderflow(paginaPai);
    }

}

void concatenarInterna(Pagina *paginaAtual, Pagina *paginaIrma, Pagina *paginaPai, BP_Tree* bp_tree) {
    //achar a posicao do filho da pagina pai que é a pagina atual
    int posAtual = 0;
    while (paginaPai->filho[posAtual] != paginaAtual->index)
        posAtual++;
    
    //encontrando a posição da chave que referencia a pagina Atual
    int posPai = (paginaPai->qtdElementos == posAtual) ? paginaPai->qtdElementos-1 : posAtual;
    //vereficar se a pagina irma é a esquerda ou a direita da pagina atual
    if (!(paginaAtual->indexProximaPagina == paginaIrma->index)){
        swap(paginaAtual, paginaIrma);
    }

    //descer o pai para a pagina atual
    paginaAtual->chave[paginaAtual->qtdElementos] = paginaPai->chave[posPai];
    paginaAtual->qtdElementos++;

    //concatenar a pagina atual com a irma, automaticamente o pai vai ganhar o filho correto
    for (int i = 0; i < paginaIrma->qtdElementos; i++)
    {
        paginaAtual->chave[paginaAtual->qtdElementos] = paginaIrma->chave[i];
        paginaAtual->filho[paginaAtual->qtdElementos] = paginaIrma->filho[i];
        paginaAtual->qtdElementos++;
    }
    //copiar o ultimo filho
    paginaAtual->filho[paginaAtual->qtdElementos] = paginaIrma->filho[paginaIrma->qtdElementos];

    //atualizando o index do pai das paginas concatenadas
    for(int i = 0; i <= paginaIrma->qtdElementos; i++){
        FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
        Pagina tempPagina;
        
        fseek(arquivoArvore, sizeof(BP_Tree) + paginaIrma->filho[i] * sizeof(Pagina), SEEK_SET);
        fread(&tempPagina, sizeof(Pagina), 1, arquivoArvore);
        
        tempPagina.pai = paginaAtual->index;
        
        fseek(arquivoArvore, sizeof(BP_Tree) + paginaIrma->filho[i] * sizeof(Pagina), SEEK_SET);
        fwrite(&tempPagina, sizeof(Pagina), 1, arquivoArvore);
        fclose(arquivoArvore);
    }
    //marca a pagina irma como deletada
    paginaIrma->foiDeletada = 1;
    bp_tree->qtdPaginas--;

    // removendo o chave que referencia a pagina atual na pagina pai
    for (int i = posPai; i < paginaPai->qtdElementos-1; i++)
    {
        paginaPai->chave[i] = paginaPai->chave[i + 1];
        paginaPai->filho[i + 1] = paginaPai->chave[i + 2];
    }
    paginaPai->filho[paginaPai->qtdElementos-1] = paginaPai->filho[paginaPai->qtdElementos];
    paginaPai->qtdElementos--;

    //verificando se a pagina pai é a raiz, se for, atualiza a raiz
    if(paginaPai->pai == -1 && paginaPai->qtdElementos == 0){
        paginaPai->foiDeletada = 1;
        bp_tree->raiz = paginaAtual->index;
        bp_tree->qtdPaginas--;
        paginaAtual->pai = -1;
    }
    
    ordenarInterna(paginaAtual);

    
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    //escrevendo o pai
    fseek(arquivoArvore, sizeof(BP_Tree) + paginaPai->index * sizeof(Pagina), SEEK_SET);
    fwrite(paginaPai, sizeof(Pagina), 1, arquivoArvore);
    //escrevendo a irma
    fseek(arquivoArvore, sizeof(BP_Tree) + paginaIrma->index * sizeof(Pagina), SEEK_SET);
    fwrite(paginaIrma, sizeof(Pagina), 1, arquivoArvore);
    //escrevendo a pagina atual
    fseek(arquivoArvore, sizeof(BP_Tree) + paginaAtual->index * sizeof(Pagina), SEEK_SET);
    fwrite(paginaAtual, sizeof(Pagina), 1, arquivoArvore);
    //escrevendo o cabeçalho da arvore
    fseek(arquivoArvore, 0, SEEK_SET);
    fwrite(bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

    fclose(arquivoArvore);
    

    //verificar se a pagina pai precisa ser concatenada
    if(paginaPai->pai != -1 && paginaPai->qtdElementos < (ORDEM-1)/2){
        fixUnderflow(paginaPai);
    }
}

void fixUnderflow(Pagina *paginaAtual){
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");

    //pega referencia da arvore
    BP_Tree bp_tree;
    fseek(arquivoArvore, 0, SEEK_SET);
    fread(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);
    
    // Calculate the position of the sibling node in the file
    int posIrma = -1, posAtual = 0;
    Pagina paginaIrma, paginaPai;

    fseek(arquivoArvore, sizeof(BP_Tree) + paginaAtual->pai * sizeof(Pagina), SEEK_SET);
    fread(&paginaPai, sizeof(Pagina), 1, arquivoArvore);

    while(paginaPai.filho[posAtual] != paginaAtual->index) posAtual++;

    if(posAtual > 0){
        posIrma = paginaPai.filho[posAtual - 1];
        fseek(arquivoArvore, sizeof(BP_Tree) + (posIrma)*sizeof(Pagina), SEEK_SET);
        fread(&paginaIrma, sizeof(Pagina), 1, arquivoArvore);
    }
    if(posAtual < paginaPai.qtdElementos && (posIrma == -1 || paginaAtual->qtdElementos + paginaIrma.qtdElementos < ORDEM)){
        posIrma = paginaPai.filho[posAtual + 1];
        fseek(arquivoArvore, sizeof(BP_Tree) + (posIrma)*sizeof(Pagina), SEEK_SET);
        fread(&paginaIrma, sizeof(Pagina), 1, arquivoArvore);
    }

    // printf("")

    if(posIrma == -1  || paginaAtual->qtdElementos + paginaIrma.qtdElementos < ORDEM){
        //concatenar
        
        printf("entrou concatenar\n");
        if(paginaAtual->tipo == FOLHA){
            printf("entrou concatenarExterna\n");
            concatenarExterna(paginaAtual, &paginaIrma, &paginaPai, &bp_tree);
            printf("p: %d qt =>  %d\n", paginaPai.index,paginaPai.qtdElementos);
        } else {
            printf("entrou concatenarInterna\n");
            concatenarInterna(paginaAtual, &paginaIrma, &paginaPai, &bp_tree);
            goto desvio;
        } 
    }
    else {
        printf("entrou redis\n");
        redistribute(paginaAtual, &paginaPai, posAtual, &paginaIrma, posIrma);
    }

    //escrevendo o pai
    fseek(arquivoArvore, sizeof(BP_Tree) + paginaPai.index * sizeof(Pagina), SEEK_SET);
    fwrite(&paginaPai, sizeof(Pagina), 1, arquivoArvore);
    //escrevendo a irma
    fseek(arquivoArvore, sizeof(BP_Tree) + paginaIrma.index * sizeof(Pagina), SEEK_SET);
    fwrite(&paginaIrma, sizeof(Pagina), 1, arquivoArvore);
    //escrevendo a pagina atual
    fseek(arquivoArvore, sizeof(BP_Tree) + paginaAtual->index * sizeof(Pagina), SEEK_SET);
    fwrite(paginaAtual, sizeof(Pagina), 1, arquivoArvore);
    

desvio:
    fclose(arquivoArvore);

}

void deletarPaciente(int id)
{

    int indexPagina;
    buscarPaciente(id, &indexPagina);

    BP_Tree bp_tree;
    Pagina pagina;
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    fseek(arquivoArvore, sizeof(BP_Tree) + indexPagina * sizeof(Pagina), SEEK_SET);
    fread(&pagina, sizeof(Pagina), 1, arquivoArvore);

    int del = 0;
    while (pagina.chave[del] != id)
        del++;

    deletarRegistros(pagina.filho[del]);

    for (int i = del; i < pagina.qtdElementos - 1; i++)
    {
        pagina.chave[i] = pagina.chave[i + 1];
        pagina.filho[i] = pagina.filho[i + 1];
    }
    pagina.qtdElementos--;

    // removendo elemento da pagina interna
    if (del == 0 && pagina.pai != -1)
    {
        atualizarPaginaInterna(pagina.pai, id, pagina.chave[0]);
    }

    

    if(pagina.pai != -1 && pagina.qtdElementos < (ORDEM+1)/2){
        fixUnderflow(&pagina);
    }

    // Diminuindo o número de pacientes
    fseek(arquivoArvore, 0, SEEK_SET);
    fread(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);
    bp_tree.qtdPacientes--;
    if(pagina.index == bp_tree.raiz && pagina.qtdElementos == 0){
        pagina.foiDeletada = 1;
        bp_tree.raiz = -1;
        bp_tree.qtdPaginas--;
    }
    fseek(arquivoArvore, 0, SEEK_SET);
    fwrite(&bp_tree, sizeof(BP_Tree), 1, arquivoArvore);

    fseek(arquivoArvore, sizeof(BP_Tree) + pagina.index * sizeof(Pagina), SEEK_SET);
    fwrite(&pagina, sizeof(Pagina), 1, arquivoArvore);

    fclose(arquivoArvore);
}

void imprimirIntervaloPacientes(int initialID, int finalID)
{
    int indexPagina;
    buscarPaciente(initialID, &indexPagina);
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    FILE *arquivoRegistros = fopen(ARQUIVO_REGISTROS, "rb+");

    Pagina pagina;
    fseek(arquivoArvore, sizeof(BP_Tree) + indexPagina * sizeof(Pagina), SEEK_SET);
    fread(&pagina, sizeof(Pagina), 1, arquivoArvore);
    // buscar elemento inicial na pagina
    int i = 0;
    while (pagina.chave[i] < initialID)
    {
        i++;
        if (i >= pagina.qtdElementos && pagina.indexProximaPagina != -1)
        {
            // buscar proxima pagina
            fseek(arquivoArvore, sizeof(BP_Tree) + pagina.filho[i] * sizeof(Pagina), SEEK_SET);
            fread(&pagina, sizeof(Pagina), 1, arquivoArvore);
            i = 0;
        }
    }
    // imprimir elementos da pagina
    while (pagina.chave[i] <= finalID)
    {
        imprimirPaciente(pagina.chave[i], pagina.index);
        i++;
        if (i >= pagina.qtdElementos && pagina.indexProximaPagina != -1)
        {
            // buscar proxima pagina
            fseek(arquivoArvore, sizeof(BP_Tree) + pagina.indexProximaPagina * sizeof(Pagina), SEEK_SET);
            fread(&pagina, sizeof(Pagina), 1, arquivoArvore);
            i = 0;
        }
        else if (i >= pagina.qtdElementos && pagina.indexProximaPagina == -1)
            break;
    }
    fclose(arquivoArvore);
    fclose(arquivoRegistros);
}

void atualizarPaginaInterna(int indexPai, int idRemovido, int novoId)
{
    printf("atualizando pagina interna foi chamado !!!\n");
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb+");
    Pagina paginaInterna;
    fseek(arquivoArvore, sizeof(BP_Tree) + indexPai * sizeof(Pagina), SEEK_SET);
    fread(&paginaInterna, sizeof(Pagina), 1, arquivoArvore);

    while (1)
    {
        int pos = 0;
        while (pos < paginaInterna.qtdElementos && paginaInterna.chave[pos] < idRemovido)
        {
            printf("%d ", paginaInterna.chave[pos]);
            pos++;
        }
        printf("\n");
        if (pos < paginaInterna.qtdElementos && paginaInterna.chave[pos] == idRemovido)
        {
            printf("achei => %d !\n", paginaInterna.index);
            paginaInterna.chave[pos] = novoId;
            break;
        }
        if (paginaInterna.pai != -1)
        {
            fseek(arquivoArvore, sizeof(BP_Tree) + paginaInterna.pai * sizeof(Pagina), SEEK_SET);
            fread(&paginaInterna, sizeof(Pagina), 1, arquivoArvore);
        }
        else
            break;
    }

    fseek(arquivoArvore, sizeof(BP_Tree) + paginaInterna.index * sizeof(Pagina), SEEK_SET);
    fwrite(&paginaInterna, sizeof(Pagina), 1, arquivoArvore);
    fclose(arquivoArvore);
}
