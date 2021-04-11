CC=gcc

CFLAGS=-c -Wall

all: mizzo

# mizzo: main.o mizzo.o 
# 	$(CC) main.o mizzo.o -o mizzo

mizzo: main.o mizzo.o threadUtils.o io.o testMizzo.o
	$(CC) -o mizzo main.o mizzo.o threadUtils.o io.o testMizzo.o -lpthread -lrt
	
main.o: main.c 
	$(CC) $(CFLAGS) main.c

mizzo.o: mizzo.c
	$(CC) $(CFLAGS) mizzo.c

threadUtils.o: threadUtils.c
	$(CC) $(CFLAGS) threadUtils.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

testMizzo.o: testMizzo.c
	$(CC) $(CFLAGS) testMizzo.c

clean:
	$(RM) *.o mizzo