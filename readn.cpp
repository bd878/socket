#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

ssize_t readn(int fd, void *vptr, size_t n) {
  size_t nleft;
  ssize_t nread;
  char* ptr;

  ptr = (char *)vptr;
  nleft = n;
  while (nleft > 0) {
    if ((nread = read(fd, ptr, nleft)) < 0) {
      if (errno == EINTR) {
        nread = 0;
      } else {
        return -1;
      }
    } else if (nread == 0) {
      break;
    }

    nleft -= nread;
    ptr += nread;
  }

  return n - nleft;
}

int main() {
  int fd;
  ssize_t bytes_readen;
  const int capacity = 13;
  char ptr[13];

  fd = open("writen.txt", O_RDONLY);
  bytes_readen = readn(fd, ptr, capacity);
  if (bytes_readen > 0) {
    printf("%s\n", ptr);
  } else {
    printf("Failed to read %d bytes\n", capacity);
  }

  return 0;
}