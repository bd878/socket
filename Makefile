daytimeclient : daytimeclient.o lib.o
	gcc -o daytimeclient.out daytimeclient.o lib.o

reusaddrclient : client_reusaddr.o lib.o readline.o
	gcc -o reusaddrclient.out client_reusaddr.o readline.o lib.o

daytimeserver : daytimeserver.o lib.o
	gcc -o daytimeserver.out daytimeserver.o lib.o

daytimeclient.o : daytimeclient.c lib.h
	gcc -c daytimeclient.c

daytimeserver.o : daytimeserver.c lib.h
	gcc -c daytimeserver.c

client_reusaddr.o : client_reusaddr.c readline.c lib.h
	gcc -c client_reusaddr.c readline.c

lib.o : lib.h lib.c
	gcc -c lib.c

clear :
	rm -rf daytimeclient.out daytimeserver.out daytimeserver.o daytimeclient.o client_reusaddr.o reusaddrclient.out lib.o
