
CAMINHO_ARVORE_OBJ = ../obj/b_plus_tree.o
CAMINHO_UTIL_OBJ = ../obj/util.o
CAMINHO_MAIN_OBJ = ../obj/main.o
CAMINHO_PAGINA_OBJ = ../obj/pagina.o

all: arvore

arvore:  clean
	 gcc -c pagina.c -o $(CAMINHO_PAGINA_OBJ) && gcc -c util.c -o $(CAMINHO_UTIL_OBJ) && gcc -c b_plus_tree.c -o $(CAMINHO_ARVORE_OBJ)  &&  gcc -c main.c -o  $(CAMINHO_MAIN_OBJ) && gcc $(CAMINHO_MAIN_OBJ) $(CAMINHO_ARVORE_OBJ)  $(CAMINHO_UTIL_OBJ) -o main && ./main
clean:
	rm -rf *.o






# Path: main/src/b_plus_tree.c


