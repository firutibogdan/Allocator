
build: allocator.o funct.o funct.h
	gcc allocator.o funct.o -o allocator

allocator.o: allocator.c
	gcc -c allocator.c

funct.o: funct.c
	gcc -c funct.c

clean:
	rm -rf allocator *.o
