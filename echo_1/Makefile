client.out : echo_client_1.o lib.o readline.o
	gcc -o client echo_client_1.o lib.o readline.o

server.out : echo_server_1.o lib.o
	gcc -o server echo_server_1.o lib.o

readline.o : ../readline.c ../lib.h
	gcc -c ../readline.c

echo_client_1.o : echo_client_1.c ../lib.h
	gcc -c ./echo_client_1.c

lib.o : ../lib.c ../lib.h
	gcc -c ../lib.c

clean :
	rm -rf client.out server.out