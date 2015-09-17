#OBJS specifies which files to compile as part of the project
OBJS = main.cpp graphics.cpp structures.cpp model.cpp

#CC specifies which compiler we're using 
CC = g++ 

COMPILER_FLAGS = -w

LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = main.out 

#This is the target that compiles our executable 
all : $(OBJS) 
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
