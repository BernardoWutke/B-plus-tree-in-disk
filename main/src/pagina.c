#include"../headers/pagina.h"

Pagina *criaPagina(int ordem){
    Pagina *pagina = (Pagina*) malloc(sizeof(Pagina));
    pagina->tipo = FOLHA;
    pagina->qtde = 0;
    pagina->pai = -1;
    pagina->ordem = ordem;
    pagina->prox = -1;
    pagina->ant = -1;
    pagina->chave = (int*) malloc(sizeof(int) *(ordem + 1));
    pagina->filho = (int*) malloc(sizeof(int) * (ordem + 1));
    return pagina;
}

void ordenarPagina(Pagina *p, int ordem){
    int j, temp1, temp2;
    for (int i = 1; i < p->qtde; i++) {
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

    p->chave[p->qtde] = chave;
    p->qtde++;
    ordenarPagina(p, p->qtde);
    if(p->qtde > p->ordem){

        Pagina *novaPagina = criaPagina(p->ordem);
        for(int j = 0, k = p->qtde/2 + 1; k < p->qtde; ++k, ++j){
            novaPagina->chave[j] = p->chave[k];
        }
        //definir pai
        if(p->pai != -1){
            FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
            fseek(fp, p->pai*sizeof(Pagina), SEEK_SET);
            Pagina *pai = (Pagina*) malloc(sizeof(Pagina));
            fread(pai, sizeof(Pagina), 1, fp); 
            fclose(fp);
            inserirElemento(pai, p->qtde/2);
            p->qtde = p->qtde/2 - 1;
        }
        else{
            Pagina *novaRaiz = criaPagina(p->ordem);
            novaRaiz->chave[0] = chave;
            novaRaiz->filho[0] = p->index;


        }
    }
}