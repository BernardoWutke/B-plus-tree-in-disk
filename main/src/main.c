#include "../headers/util.h"
#include "../headers/b_plus_tree.h"
int num_paciente = 0;

void imprimirMenu()
{
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

void flush_in()
{
    int ch;
    while ((ch = fgetc(stdin)) != EOF && ch != '\n')
    {
    }
}

Paciente lerDadosPaciente()
{
    Paciente novoPaciente;

    int _;
    printf("Digite o ID do paciente: ");
    scanf("%d", &novoPaciente.id);
    while (buscarPaciente(novoPaciente.id, &_))
    {
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

/* void teste(){
    Paciente p;
    for(int i = 0; i < 13; i++){
        p.id = num_paciente++;
        strcpy(p.nome, "a");
        p.anoNascimento = 1;
        strcpy(p.CPF, "a");
        strcpy(p.endereco, "a");
        strcpy(p.nomeMae, "a");
        strcpy(p.nomePai, "a");
        inserirPaciente(p);
    }
    deletarPaciente(4);
    deletarPaciente(1);
    deletarPaciente(2);
    deletarPaciente(10);
    deletarPaciente(3);
    deletarPaciente(5);
    deletarPaciente(7);
    deletarPaciente(9);
    deletarPaciente(6);
    deletarPaciente(12);
    deletarPaciente(8);
    deletarPaciente(0);
    deletarPaciente(11);

} */

int main()
{
    BP_Tree bp_tree;
    inicializarBP(); // carrega o cabeçalho da arvore

    int opcao = -1;

    int id, indexPagina;

    teste();

    while (opcao != 6)
    {
        imprimirMenu();
        printf("\nDigite sua opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1)
        { // buscar paciente pelo ID
            printf("\nDigite o id do paciente: ");
            scanf("%d", &id);
            if (!buscarPaciente(id, &indexPagina))
            {
                printf("Paciente com ID %d nao existe!\n", id);
            }
            else
            {
                imprimirPaciente(id, indexPagina);
            }
        }
        else if (opcao == 2)
        { // Cadastrar paciente
            inserirPaciente(lerDadosPaciente());
        }
        else if (opcao == 3)
        {
            printf("\nDigite o id do paciente: ");
            scanf("%d", &id);
            if (!buscarPaciente(id, &indexPagina))
            {
                printf("Paciente com ID %d nao existe!\n", id);
            }
            else
            {
                char c;
                imprimirPaciente(id, indexPagina);
                printf("Tem certeza que deseja excluir? (Y/N) ");
                flush_in();
                scanf("%c", &c);
                if (c == 'y' || c == 'Y')
                {
                    deletarPaciente(id);
                    printf("\nPaciente %d removido com sucesso!\n\n", id);
                }
                else
                    printf("\nOperação cancelada, paciente não foi removido!\n\n");
            }
        }
        else if (opcao == 4)
        {
            imprimirArvore();
        }
        else if (opcao == 5)
        {
            int initialID, finalID;
            printf("Digite o primeiro ID: ");
            scanf("%d", &initialID);
            while (initialID < 0)
            {
                printf("Valor inválido, digite novamente: ");
                scanf("%d", &initialID);
            }
            printf("Digite o último ID: ");
            scanf("%d", &finalID);
            while (finalID < 0 || finalID < initialID)
            {
                printf("Valor inválido, digite novamente: ");
                scanf("%d", &finalID);
            }
            imprimirIntervaloPacientes(initialID, finalID);
        }
        else if (opcao == 6)
        {
            printf("Sistema encerrado.\n");
        }
        else
        {
            printf("\nValor invalido!\n");
        }
    }
    return 0;
}