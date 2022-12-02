
CAMINHO_ARVORE_OBJ = ../main/obj/b_plus_tree.o
CAMINHO_UTIL_OBJ = ../main/obj/util.o
CAMINHO_MAIN_OBJ = ../main/obj/main.o
CAMINHO_PAGINA_OBJ = ../main/obj/pagina.o

CAMINHO_ARVORE = ../main/src/b_plus_tree.c
CAMINHO_UTIL = ../main/src/util.c
CAMINHO_MAIN = ../main/src/main.c
CAMINHO_PAGINA = ../main/src/pagina.c

all: arvore

arvore:  clean
	 gcc -c $(CAMINHO_PAGINA) -o $(CAMINHO_PAGINA_OBJ) && gcc -c $(CAMINHO_UTIL) -o $(CAMINHO_UTIL_OBJ) && gcc -c $(CAMINHO_ARVORE) -o $(CAMINHO_ARVORE_OBJ)  &&  gcc -c $(CAMINHO_MAIN) -o  $(CAMINHO_MAIN_OBJ) && gcc $(CAMINHO_MAIN_OBJ) $(CAMINHO_ARVORE_OBJ)  $(CAMINHO_UTIL_OBJ) -o main && ./main
clean:
	rm -rf *.o

