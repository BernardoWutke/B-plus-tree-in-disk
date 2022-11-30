#ifndef _UTIL_H_
#define _UTIL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


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

#define ARQUIVO_ARVORE "arvore.bin"

#endif 

