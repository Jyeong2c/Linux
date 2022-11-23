CFLAGS=-g

all: input.o print.o main.o
	gcc -g -ggdb -o test main.o input.o print.o

clean:
	rm *.o test
