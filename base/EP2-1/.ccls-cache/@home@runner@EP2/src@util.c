#include "../headers/util.h"

#define ERROR(msg) fprintf(stderr, "[!]: %s\n", #msg)

void *
mallocSafe (size_t nbytes) {
  void *ptr = malloc (nbytes);
  
  if (ptr == NULL) {
    ERROR(erro ao alocar memória.);
    exit(EXIT_FAILURE);
  }

  return ptr;
}
