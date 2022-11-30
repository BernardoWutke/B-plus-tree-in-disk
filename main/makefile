DIRMATHEUS = -I/mnt/d/Ciencia-da-Computacao/'Semestre 5'/ED2/Bplus/arvore-b-plus/main/headers

link:
	gcc -o sistemaHospital b_plus_tree.o main.o


BPTree-matheus:
	gcc -c $(DIRMATHEUS) src/b_plus_tree.c -o b_plus_tree.o

main-matheus: 
	gcc -c $(DIRMATHEUS) src/main.c -o main.o


matheus: BPTree-matheus main-matheus link
	./sistemaHospital