CC=gcc
CFLAGS=-lm
OBJ=main.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

mainmake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


clean:

