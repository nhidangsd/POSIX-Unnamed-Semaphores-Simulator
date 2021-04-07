CC=gcc

CFLAGS=-c -Wall

all: mizzo

# mizzo: main.o mizzo.o 
# 	$(CC) main.o mizzo.o -o mizzo

mizzo: main.o mizzo.o io.o testMizzo.o
	$(CC) main.o mizzo.o io.o testMizzo.o -o mizzo

testMizzo.o: testMizzo.c
	$(CC) $(CFLAGS) testMizzo.c
	
main.o: main.c 
	$(CC) $(CFLAGS) main.c

mizzo.o: mizzo.c
	$(CC) $(CFLAGS) mizzo.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

clean:
	$(RM) *.o mizzo