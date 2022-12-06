#include "../headers/util.h"
#include "../headers/b_plus_tree.h"

void imprimirMenu(){
    printf("--------------------------------\n");
    printf("|       SISTEMA DE SAUDE       |\n");
    printf("--------------------------------\n");
    printf("| [ 1 ] BUSCAR PACIENTE        |\n");
    printf("| [ 2 ] CADASTRAR PACIENTE     |\n");
    printf("| [ 3 ] REMOVER PACIENTE       |\n");
    printf("| [ 4 ] IMPRIMIR ARVORE        |\n");
    printf("| [ 5 ] INTERVALO DE PACIENTES |\n");
    printf("| [ 6 ] SAIR                   |\n");
    printf("--------------------------------\n");
}

Paciente lerDadosPaciente(){
    Paciente novoPaciente;

    printf("Digite o nome do paciente: ");
    scanf("%s", novoPaciente.nome);
    printf("Digite o ano de nascimento do paciente: ");
    scanf("%d", &novoPaciente.anoNascimento);
    printf("Digite o CPF do paciente: ");
    scanf("%s", novoPaciente.CPF);
    printf("Digite o endereco do paciente: ");
    scanf("%s", novoPaciente.endereco);
    printf("Digite o nome da Mae do paciente: ");
    scanf("%s", novoPaciente.nomeMae);
    printf("Digite o nome do Pai do paciente: ");
    scanf("%s", novoPaciente.nomePai);

    return novoPaciente;
}

int main(){

    BP_Tree bp_tree;
    inicializarBP(); // carrega o cabeçalho da arvore

    int opcao = -1;

    int id, indexPagina;

    while(opcao != 6){
        imprimirMenu();
        printf("\nDigite sua opcao: ");
        scanf("%d", &opcao);
        
        if(opcao == 1){ // buscar paciente pelo ID 
            printf("\nDigite o id do paciente: ");
            scanf("%d", &id);
            if(!buscarElemento(id, &indexPagina)){
                printf("Paciente com ID %d nao existe!\n", id);
            }
            else{
                imprimirPaciente(id, indexPagina);
            }
        }
        else if(opcao == 2){ // Cadastrar paciente
            inserirPaciente(lerDadosPaciente());
        }
        else if(opcao == 3){

        }
        else if(opcao == 4){
            imprimirArvoreHeader();

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