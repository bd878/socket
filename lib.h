#ifndef __lib_h
#define __lib_h

#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int Socket(int, int, int);
void Bind(int, const struct sockaddr *, size_t);
void Listen(int, int);
int Accept(int, struct sockaddr *, socklen_t *);
void Connect(int, const struct sockaddr *, socklen_t);
pid_t Fork();
void Close(int);
void Writen(int, const void *, size_t);
ssize_t Readline(int, void *, size_t);
char *Fgets(char *, int, FILE *);
void Fputs(const char *, FILE *);
void Inet_pton(int, const char *, void *);

#endif