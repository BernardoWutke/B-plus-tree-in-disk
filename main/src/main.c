#include "../headers/util.h"
#include "../headers/b_plus_tree.h"
char num_paciente = 'a';

void imprimirMenu(){
    printf("------------------------------------------------------\n");
    printf("|                  SISTEMA DE SAUDE                  |\n");
    printf("------------------------------------------------------\n");
    printf("|                                                    |\n");
    printf("|  [ 1 ] BUSCAR PACIENTE                             |\n");
    printf("|  [ 2 ] CADASTRAR PACIENTE                          |\n");
    printf("|  [ 3 ] REMOVER PACIENTE                            |\n");
    printf("|  [ 4 ] IMPRIMIR ARVORE                             |\n");
    printf("|  [ 5 ] INTERVALO DE PACIENTES                      |\n");
    printf("|  [ 6 ] SAIR                                        |\n");
    printf("|                                                    |\n");
    printf("------------------------------------------------------\n");
}

void flush_in(){ 
    int ch;
    while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){}
}

Paciente lerDadosPaciente(){
    Paciente novoPaciente;

    int _;
    printf("Digite o ID do paciente: ");
    scanf("%d", &novoPaciente.id);
    while(buscarPaciente(novoPaciente.id, &_)){
        printf("ID ja cadastrado! digite outro ID: ");
        scanf("%d", &novoPaciente.id);
    }
    
    flush_in();
    printf("Digite o nome do paciente: ");
    scanf("%[^\n]", novoPaciente.nome);

    printf("Digite o ano de nascimento do paciente: ");
    scanf("%d", &novoPaciente.anoNascimento);

    flush_in();
    printf("Digite o CPF do paciente: ");
    scanf("%s", novoPaciente.CPF);

    flush_in();
    printf("Digite o endereco do paciente: ");
    scanf("%[^\n]", novoPaciente.endereco);

    flush_in();
    printf("Digite o nome da Mae do paciente: ");
    scanf("%[^\n]", novoPaciente.nomeMae);

    flush_in();
    printf("Digite o nome do Pai do paciente: ");
    scanf("%[^\n]", novoPaciente.nomePai);

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
            if(!buscarPaciente(id, &indexPagina)){
                printf("Paciente com ID %d nao existe!\n", id);
            }
            else{
                imprimirPaciente(id, indexPagina);
            }
        }
        else if(opcao == 2){ // Cadastrar paciente
            // num_paciente++;
            // Paciente p;
            // strcpy(p.nome, "a");
            // p.anoNascimento = 1;
            // strcpy(p.CPF, "a");
            // strcpy(p.endereco, "a");
            // strcpy(p.nomeMae, "a");
            // strcpy(p.nomePai, "a");
            // inserirPaciente(p);

            inserirPaciente(lerDadosPaciente());
        }
        else if(opcao == 3){
            printf("\nDigite o id do paciente: ");
            scanf("%d", &id);
            if(!buscarPaciente(id, &indexPagina)){
                printf("Paciente com ID %d nao existe!\n", id);
            }
            else{
                char c;
                imprimirPaciente(id, indexPagina);
                printf("Tem certeza que deseja excluir? (Y/N) ");
                flush_in();
                scanf("%c", &c);
                if(c == 'y' || c == 'Y'){
                    deletarPaciente(id);
                }
            }
        }
        else if(opcao == 4){
            imprimirArvore();

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