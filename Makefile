daytimeclient : daytimeclient.o lib.o
	gcc -o daytimeclient.out daytimeclient.o lib.o

daytimeserver : daytimeserver.o lib.o
	gcc -o daytimeserver.out daytimeserver.o lib.o

daytimeclient.o : daytimeclient.c lib.h
	gcc -c daytimeclient.c

daytimeserver.o : daytimeserver.c lib.h
	gcc -c daytimeserver.c

lib.o : lib.h lib.c
	gcc -c lib.c

clear :
	rm -rf daytimeclient.out daytimeserver.out daytimeserver.o daytimeclient.o lib.o
