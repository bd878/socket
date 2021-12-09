limits-static : limits.o
	gcc -static -o ./limits.out ./limits.o

setnofile : setnofile.o
	gcc -o ./setnofile.out ./setnofile.o

nofile : nofile.c
	gcc -o ./nofile.out ./nofile.c

limits : limits.o
	gcc -o ./limits.out ./limits.o

limits.o : limits.c
	gcc -c ./limits.c

setnofile.o : setnofile.c
	gcc -c ./setnofile.c

clear :
	rm -rf ./limits.out ./limits.o ./setnofile.out ./setnofile.o, ./nofile.o