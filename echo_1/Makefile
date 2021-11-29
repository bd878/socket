client : echo_client_1.o lib.o readline.o
	gcc -o client.out echo_client_1.o lib.o readline.o

server : echo_server_1.o lib.o readline.o
	gcc -o server.out echo_server_1.o lib.o readline.o

client_select : echo_client_select.o lib.o readline.o
	gcc -o client_select.out echo_client_select.o lib.o readline.o

server_select : echo_server_select.o lib.o
	gcc -o server_select.out echo_server_select.o lib.o

readline.o : ../readline.c ../lib.h
	gcc -c ../readline.c

echo_client_1.o : echo_client_1.c ../lib.h
	gcc -c ./echo_client_1.c

echo_client_select.o : echo_client_select.c ../lib.h
	gcc -c ./echo_client_select.c

echo_server_select.o : echo_server_select.c ../lib.h
	gcc -c ./echo_server_select.c

lib.o : ../lib.c ../lib.h
	gcc -c ../lib.c

clean :
	rm -rf client.out server.out