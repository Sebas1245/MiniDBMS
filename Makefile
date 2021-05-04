CFLAGS = -O
CC = gcc
SRC =  ./jsonparser/mjson/mjson.c ./jsonparser/structsmappings.c  server.c
OBJ = $(SRC:.c = .o)

miniDBMS: $(OBJ)
	$(CC) $(CFLAGS) -o server $(OBJ) 

