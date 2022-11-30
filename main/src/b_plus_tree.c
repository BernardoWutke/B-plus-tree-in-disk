#include<b_plus_tree.h>

void inicializarBP(BP_Tree *bp){
    // Tenta ler o arquivo base
    FILE *arquivoArvore = fopen(ARQUIVO_ARVORE, "rb");

    // Se já tiver arquivo, carrega o cabeçalho em bp
    if (arquivoArvore != NULL) {
        fread(&bp, sizeof(BP_Tree), 1, arquivoArvore);
        fclose(arquivoArvore);
    }
    else{ // Se não cria um novo arquivo com um cabeçalho do zero
        bp->ordem = ORDEM;
        bp->qtde = 0;
        bp->raiz = -1;
        arquivoArvore = fopen(ARQUIVO_ARVORE, "wb+");
        fwrite(&bp, sizeof(BP_Tree), 1, arquivoArvore);
        fclose(arquivoArvore);
    }
}

