#ifndef __lib_h
#define __lib_h

#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/time.h>
#include <fcntl.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#ifndef INFTIM
#define INFTIM -1
#endif

typedef void Sigfunc(int);

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
FILE *Fopen(const char *, const char *);
FILE *Fdopen(int, const char *);
FILE *Freopen(const char *, const char *, FILE *);
void Inet_pton(int, const char *, void *);
Sigfunc *Signal(int, Sigfunc *);
void Setfl(int fd, int);
void Clrfl(int fd, int);
ssize_t Readn(int, void *, size_t);
ssize_t Read(int, void *, size_t);
int Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int Poll(struct pollfd *, unsigned long, int);
void Shutdown(int, int);
ssize_t Write(int, const void *, size_t);

#endif