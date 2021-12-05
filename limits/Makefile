limits-static : limits.o
	gcc -static -o ./limits.out ./limits.o

limits : limits.o
	gcc -o ./limits.out ./limits.o

limits.o : limits.c
	gcc -c ./limits.c

clear :
	rm -rf ./limits