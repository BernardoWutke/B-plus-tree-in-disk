# Nome do projeto
PROJECT = EP2

# Definição dos diretórios dinâmicos
DIR_OUT = obj
DIR_SRC += .
DIR_SRC = ./src
DIR_INC += $(addprefix -I, $(DIR_SRC))

C_SOURCE += $(wildcard $(addsuffix /*.c, $(DIR_SRC)))
OBJ := $(patsubst %.c, %.o, $(C_SOURCE))

# Definição do compilador
CC = gcc

# Arquivo executável
EXE := obj/$(PROJECT)
 
# Definição das flags de compilação
CC_FLAGS = 	-c	  	\
            -Wall		\
            -lm 		\
            -O1	  \
            -std=c11	\
            -Wextra	\
            -Wshadow  \
            -Wmaybe-uninitialized   \
            -Wno-sign-compare		\
            -Wno-error=address \
            -Wno-unused-result  \
            -Wno-pointer-to-int-cast  \
            -Wincompatible-pointer-types  \
            -Wno-unused-parameter		\
            -Wno-unused-variable		

# Comandos para limpeza recursiva dos arquivos 
# remanescentes na pasta src/		 
RM = rm -rf
MD = $(shell mkdir -p $(DIR_OUT))

# Linkagem e Compilação
.PHONY:all

all:$(OBJ) $(EXE)
%.o: %.c
	$(MD)
	$(CC) $(DIR_INC) $(CC_FLAGS) $< -o $@

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(DIR_INC)

	$(RM) $(DIR_SRC)/*.o

# Limpa os arquivos objetos
clean:
	cd obj && $(RM) $(PROJECT)

# Vefica vazamentos de memória
memcheck: $(EXE)
	make && cd obj && valgrind --tool=memcheck --track-origins=yes --leak-check=full ./${PROJECT}

# Executa o programa
run: $(EXE)
	cd obj && ./${PROJECT}