
CAMINHO_ARVORE_OBJ = ../obj/b_plus_tree.o
CAMINHO_UTIL_OBJ = ../obj/util.o
CAMINHO_MAIN_OBJ = ../obj/main.o
CAMINHO_PAGINA_OBJ = ../obj/pagina.o

CAMINHO_ARVORE = ../src/b_plus_tree.c
CAMINHO_UTIL = ../src/util.c
CAMINHO_MAIN = ../src/main.c
CAMINHO_PAGINA = ../src/pagina.c

all: arvore

arvore:  clean
	 gcc -c $(CAMINHO_PAGINA) -o $(CAMINHO_PAGINA_OBJ) && gcc -c $(CAMINHO_UTIL) -o $(CAMINHO_UTIL_OBJ) && gcc -c $(CAMINHO_ARVORE) -o $(CAMINHO_ARVORE_OBJ)  &&  gcc -c $(CAMINHO_MAIN) -o  $(CAMINHO_MAIN_OBJ) && gcc $(CAMINHO_MAIN_OBJ) $(CAMINHO_ARVORE_OBJ) $(CAMINHO_PAGINA_OBJ) $(CAMINHO_UTIL_OBJ) -o main && ./main
clean:
	rm -rf *.o






# Path: main/src/b_plus_tree.c


