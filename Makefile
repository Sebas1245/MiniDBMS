CFLAGS = -O
CC = gcc

SRC =  ./jsonparser/mjson/mjson.c ./jsonparser/structsmappings.c ./queryLanguage/query_main.c  server.c
OBJ = $(SRC:.c = .o)

miniDBMS: $(OBJ)
	$(CC) $(CFLAGS) -o server.o $(OBJ) 

