#ifndef UTIL_H_
#define UTIL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ARQUIVO_ARVORE "./files/arvore.bin"  

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
} Paciente;

<<<<<<< Updated upstream
=======
void imprimirArquivo();

#define ARQUIVO_ARVORE "../binarios/arvore.bin"
>>>>>>> Stashed changes

#endif 

