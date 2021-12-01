hostent : hostent.o lib.o
	gcc -o hostent.out hostent.o lib.o

hostent.o : hostent.c ../lib.h
	gcc -c hostent.c

lib.o : ../lib.c ../lib.h
	gcc -c ../lib.c

clear :
	rm -rf hostent.o hostent.out
