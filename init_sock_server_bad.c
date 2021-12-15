#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int
init_server_socket(int type, const struct sockaddr *addr, socklen_t slen, int qlen) {
  int fd;
  int err = 0;

  if ((fd = socket(addr->sa_family, type, 0)) != -1) {
    if (bind(fd, addr, slen) != -1) {
      if (type == SOCK_STREAM || type == SOCK_SEQPACKET != -1) {
        if (listen(fd, qlen) != -1) {
          return fd;
        }
      }
    }
  }

  err = errno;
  close(fd);
  errno = err;
  return -1;
}