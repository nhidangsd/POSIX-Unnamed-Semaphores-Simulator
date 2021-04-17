CC=gcc

CFLAGS=-c -Wall -g

all: mizzo

mizzo: main.o mizzo.o threadUtils.o io.o 
	$(CC) -o mizzo main.o mizzo.o threadUtils.o io.o -lpthread -lrt
	
main.o: main.c 
	$(CC) $(CFLAGS) main.c

mizzo.o: mizzo.c
	$(CC) $(CFLAGS) mizzo.c

threadUtils.o: threadUtils.c
	$(CC) $(CFLAGS) threadUtils.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

clean:
	$(RM) *.o mizzo