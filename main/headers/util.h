#ifndef UTIL_H_
#define UTIL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define ARQUIVO_ARVORE "../binarios/arvore.bin"
#define ARQUIVO_REGISTROS "../binarios/registros.bin"
#define ORDEM 4


// Path: main/headers/ultil.c

typedef enum {
    false,
    true
} bool;

typedef struct paciente{
    int id;
    int anoNascimento;
    char nome[200];
    char endereco[500];
    char nomeMae[200];
    char nomePai[200];
    char CPF[20];
    int foiDeletado;
    
} Paciente;

void imprimirArquivo();
void imprimirPaciente(int id, int indexPagina);
void fillLine(int size, char c);


#endif 

