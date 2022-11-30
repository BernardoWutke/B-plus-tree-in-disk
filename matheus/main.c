#include <stdlib.h>
#include <stdio.h>

int main(){
    FILE *arq;
    arq = fopen("teste.bin", "ab+");

    int v[5] = {1,2,3,4,5};
    int n[5];

    fwrite(v, sizeof(int), 5, arq);
    fread(n, sizeof(int), 5, arq);

    fclose(arq);

    for(int i = 0; i < 5; ++i){
        printf("%d\n", n[i]);
    }
}