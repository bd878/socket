#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

ssize_t writen(int fd, const void *vptr, size_t n) {
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = (const char*)vptr;
  nleft = n;
  while (nleft > 0) {
    if ((nwritten = write(fd, ptr, nleft)) <= 0) {
      if (errno = EINTR) {
        nwritten = 0;
      } else {
        return -1;
      }
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return n;
}

int main() {
  int fd;
  ssize_t bytes_written;
  const char *phrase = "Hello, world!\0";

  fd = open("writen.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  bytes_written = writen(fd, phrase, strlen(phrase));
  if (bytes_written > 0) {
    printf("Bytes written: %ld", bytes_written);
  } else {
    printf("Failed to write %ld bytes\n", strlen(phrase));
  }

  return 0;
}