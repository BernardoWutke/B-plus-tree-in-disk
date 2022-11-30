#ifndef ARQUIVO_H_
#define ARQUIVO_H_

#include "bplustree.h"
#include "util.h"

typedef struct tabela {
  int qtde;
} Tabela;

Tabela *criarTabela();
void criarTabelaRegistros(char*);
void importarTabela();

#endif