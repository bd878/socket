noblock : noblock.o lib.o
	gcc -o noblock.out noblock.o lib.o

noblock.o : noblock.c ../lib.h
	gcc -c noblock.c

lib.o : ../lib.c ../lib.h
	gcc -c ../lib.c

clear :
	rm -rf noblock.out
