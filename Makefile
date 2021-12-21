daytimeclient : daytimeclient.o lib.o
	gcc -o daytimeclient.out daytimeclient.o lib.o

daytimeclient.o : daytimeclient.c lib.h
	gcc -c daytimeclient.c

lib.o : lib.h lib.c
	gcc -c lib.c

clear :
	rm -rf daytimeclient.out daytimeclient.o lib.o
