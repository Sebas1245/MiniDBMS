CFLAGS = -O
CC = gcc
SRC =  ./jsonparser/mjson/mjson.c ./jsonparser/structsmappings.c ./queryLanguage/query_main.c  main.c
OBJ = $(SRC:.c = .o)

miniDBMS: $(OBJ)
	$(CC) $(CFLAGS) -o miniDBMS $(OBJ) 

