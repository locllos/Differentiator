NAME = differentiator
MAIN_DIR = mainFiles
OBJ_DIR = objectsFiles
COMPILATOR = g++
BIN_NAME = differ.out


$(NAME): $(OBJ_DIR)/$(NAME)Main.o $(OBJ_DIR)/$(NAME)Funcs.o 
	$(COMPILATOR) $(OBJ_DIR)/$(NAME)Main.o $(OBJ_DIR)/$(NAME)Funcs.o   -o $(BIN_NAME)

$(OBJ_DIR)/$(NAME)Main.o: $(MAIN_DIR)/$(NAME)Main.cpp
	$(COMPILATOR) -c $(MAIN_DIR)/$(NAME)Main.cpp -o $(OBJ_DIR)/$(NAME)Main.o

$(OBJ_DIR)/$(NAME)Funcs.o: $(MAIN_DIR)/$(NAME)Funcs.cpp
	$(COMPILATOR) -c $(MAIN_DIR)/$(NAME)Funcs.cpp -o $(OBJ_DIR)/$(NAME)Funcs.o
