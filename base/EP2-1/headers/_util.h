#ifndef UTIL_H_
#define UTIL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

// Importações e constantes globais
#define TXT_FILE "../files/artigos.txt"
#define REG_FILE "../files/tabela_registros.bin"
#define TREE_FILE "../files/arvore.bin"
#define D 2
#define MAX (2 * D + 1)
#define TAM 200

 // Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

// Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

// Background color
#define BLKHB "\e[0;100m"
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define YELHB "\e[0;103m"
#define BLUHB "\e[0;104m"
#define MAGHB "\e[0;105m"
#define CYNHB "\e[0;106m"
#define WHTHB "\e[0;107m"

// High intensty text
#define HBLK "\e[0;90m"
#define HRED "\e[0;91m"
#define HGRN "\e[0;92m"
#define HYEL "\e[0;93m"
#define HBLU "\e[0;94m"
#define HMAG "\e[0;95m"
#define HCYN "\e[0;96m"
#define HWHT "\e[0;97m"

// Reset text
#define reset "\e[0m"

// Debugger
#define DEBUG

#define LOG(x, fmt, ...) if (x) { \
  printf("%s:%d: " fmt "\n", __FILE__, __LINE__,__VA_ARGS__); \
}

#define TRY(x,s) if(!(x)) { \
  printf("%s:%d:%s",__FILE__, __LINE__,s); \
}

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) if(!(n)) { \
                  printf("%s - Failed ",#n); \
                  printf("On %s ",__DATE__); \
                  printf("At %s ",__TIME__); \
                  printf("In File %s ",__FILE__); \
                  printf("At Line %d\n",__LINE__); \
                  return(-1);}
#endif

// Estruturas
typedef struct pagina Pagina;
typedef struct bptree BPTree;

typedef enum {
  false,
  true
} bool;

typedef struct artigo {
  int id;
  int ano;
  char autor[TAM];
  char titulo[TAM];
  char revista[TAM];
  char DOI[TAM];
  char palavraChave[TAM];
} Artigo;

void *mallocSafe(size_t);
void imprimirArtigo(int);

#endif
