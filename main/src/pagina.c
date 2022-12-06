#include"../headers/pagina.h"

Pagina *criaPagina(int ordem){
    Pagina *pagina = (Pagina*) malloc(sizeof(Pagina));
    pagina->tipo = FOLHA;
    pagina->qtdElementos = 0;
    pagina->pai = -1;
    pagina->ordem = ordem;
    pagina->indexProximaPagina = -1;
    pagina->indexPaginaAnterior = -1;

    return pagina;
}

void inicializarPagina(Pagina *pagina, int _ordem, int _tipo){
    pagina->qtdElementos = 0;
    pagina->pai = -1;
    pagina->ordem = _ordem;
    pagina->indexProximaPagina = -1;
    pagina->indexPaginaAnterior = -1;
    pagina->tipo = _tipo;
}

void ordenarPagina(Pagina *p, int ordem){
    int j, temp1, temp2;
    for (int i = 1; i < p->qtdElementos; i++) {
        temp1 = p->chave[i];
        temp2 = p->filho[i];
        for (j = i; j > 0 && temp1 < p->chave[j - 1]; j--) {
            p->chave[j] = p->chave[j - 1];
            p->filho[j] = p->filho[j - 1];
        }
        p->chave[j] = temp1;
        p->filho[j] = temp2;
    }
}

void inserirElemento(Pagina *p, int chave){

    p->chave[p->qtdElementos] = chave;
    p->qtdElementos++;
    ordenarPagina(p, p->qtdElementos);
    if(p->qtdElementos > p->ordem){

        Pagina *novaPagina = criaPagina(p->ordem);
        for(int j = 0, k = p->qtdElementos/2 + 1; k < p->qtdElementos; ++k, ++j){
            novaPagina->chave[j] = p->chave[k];
        }
        //definir pai
        if(p->pai != -1){
            FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
            fseek(fp, p->pai*sizeof(Pagina), SEEK_SET);
            Pagina *pai = (Pagina*) malloc(sizeof(Pagina));
            fread(pai, sizeof(Pagina), 1, fp); 
            fclose(fp);
            inserirElemento(pai, p->qtdElementos/2);
            p->qtdElementos = p->qtdElementos/2 - 1;
        }
        else{
            Pagina *novaRaiz = criaPagina(p->ordem);
            novaRaiz->chave[0] = chave;
            novaRaiz->filho[0] = p->index;


        }
    }
}