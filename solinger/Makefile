client : echo_client.o lib.o readline.o
	gcc -o client.out echo_client.o lib.o readline.o

server : echo_server.o lib.o readline.o
	gcc -o server.out echo_server.o lib.o readline.o

readline.o : ../readline.c ../lib.h
	gcc -c ../readline.c

echo_client.o : echo_client.c ../lib.h
	gcc -c ./echo_client.c

lib.o : ../lib.c ../lib.h
	gcc -c ../lib.c

clean :
	rm -rf client.out server.out