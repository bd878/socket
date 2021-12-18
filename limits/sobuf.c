#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

void
get_buf_sizes(int fd, int *recvbuf, int *sndbuf) {
  int rcvlen, sndlen;

  rcvlen = sizeof(*recvbuf);
  if (getsockopt(fd, SOL_SOCKET, SO_RCVBUF, recvbuf, &rcvlen) == -1) {
    perror("tcp rcvbuf getsockopt");
  }

  sndlen = sizeof(*sndbuf);
  if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, sndbuf, &sndlen) == -1) {
    perror("tcp sndbuf getsockopt");
  }
}

struct bufsize {
  int fd;
  int recvbuf;
  int sndbuf;
};

int
main() {
  struct bufsize tcpbufsize;
  struct bufsize udpbufsize;
  struct bufsize sctpbufsize;

  tcpbufsize.fd  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  udpbufsize.fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  sctpbufsize.fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

  get_buf_sizes(tcpbufsize.fd, &tcpbufsize.recvbuf, &tcpbufsize.sndbuf);
  get_buf_sizes(udpbufsize.fd, &udpbufsize.recvbuf, &udpbufsize.sndbuf);
  get_buf_sizes(sctpbufsize.fd, &sctpbufsize.recvbuf, &sctpbufsize.sndbuf);

  printf("TCP SO_RCVBUF = %d, SO_SNDBUF = %d\n", tcpbufsize.recvbuf, tcpbufsize.sndbuf);
  printf("UDP SO_RCVBUF = %d, SO_SNDBUF = %d\n", udpbufsize.recvbuf, udpbufsize.sndbuf);
  printf("SCTP SO_RCVBUF = %d, SO_SNDBUF = %d\n", sctpbufsize.recvbuf, sctpbufsize.sndbuf);

  exit(0);
}