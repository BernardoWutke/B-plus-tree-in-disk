#ifndef UTIL_H_
#define UTIL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define SIZE_TAB(path) BPTreeTamanhoTabela(path)

#define INPUT_FILE  "../files/input.in"
#define OUTPUT_FILE "../files/output.out"

#define D 2

#define MAX (2 * D + 1)

typedef enum {
  false,
  true
} bool;

typedef struct artigo{
  int id;
  int ano;
  char autor[200];
  char titulo[200];
  char revista[200];
  char DOI[20];
  char palavraChave[200];
} Artigo;

void *mallocSafe(size_t);

#endif


/*
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    if (!strncmp(argv[i], "--hello", 7)) {
      printf("hello\n");
    }

    else if (!strncmp(argv[i], "--hi", 4)) {
      printf("hi\n"); ++i;
      printf("key: %s\n", argv[i]); ++i;
      printf("value: %s\n", argv[i]);
    }

    else {
      fprintf(stderr, "usage: %s [--hello | --hi]\n", argv[0]);
      exit(1);
    }
  }

  return 0;
}
*/
