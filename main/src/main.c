#include "../headers/util.h"
#include "../headers/b_plus_tree.h"

int main(){

    BP_Tree bp_tree; 
    inicializarBP(&bp_tree); // carrega o cabeçalho da arvore

    int opcao = -1;

    while(opcao != 6){
        printf("--------------------------------\n");
        printf("|       SISTEMA DE SAUDE       |\n");
        printf("--------------------------------\n");
        printf("| [ 1 ] BUSCAR PACIENTE        |\n");
        printf("| [ 2 ] CADASTRAR PACIENTE     |\n");
        printf("| [ 3 ] REMOVER PACIENTE       |\n");
        printf("| [ 4 ] REMOVER PACIENTE       |\n");
        printf("| [ 5 ] INTERVALO DE PACIENTES |\n");
        printf("| [ 6 ] SAIR                   |\n");
        printf("--------------------------------\n");
        printf("\nDigite sua opcao: ");
        scanf("%d", &opcao);
        
        if(opcao == 1){
            int id, indexPagina;
            printf("\nDigite o id do paciente: ");
            scanf("%d", &id);
            if(!buscarElemento(id, &indexPagina, bp_tree)){
                printf("Paciente com ID %d nao existe!\n", id);
            }
            else{
                imprimirPaciente(id, indexPagina);
            }
        }
        else if(opcao == 2){

        }
        else if(opcao == 3){

        }
        else if(opcao == 4){

        }
        else if(opcao == 5){

        }
        else if(opcao == 6){
            printf("Sistema encerrado.\n");
        }
        else{
            printf("\nValor invalido!\n");
        }
    }

    return 0;
}