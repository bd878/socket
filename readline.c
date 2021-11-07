#include "lib.h"

static char _buf[1024];
static int bytes_read;
static char *buf_ptr;

static size_t
read_char(int fd, char *str) {
  if (bytes_read <= 0) {
repeat:
    if ((bytes_read = read(fd, _buf, sizeof(_buf))) < 0) {
      if (errno == EINTR) {
        goto repeat;
      } else {
        return -1;
      }
    } else if (bytes_read == 0) {
      return 0;
    }
    buf_ptr = _buf;
  }

  bytes_read--;
  *str = *buf_ptr++;
  return 1;
}

ssize_t
readline(int fd, void *vptr, size_t len) {
  size_t n, rc;
  char c, *ptr;

  ptr = vptr;
  for (n = 0; n < len; ++n) {
    if ((rc = read_char(fd, &c)) <= 0) {
      if (rc == 0) {
        *ptr = 0;
        return n-1;
      } else {
        return -1;
      }
    } else {    
      *ptr++ = c;
      if (c == '\n') {
        *ptr = 0;
        return n;
      }
    }
  }

  *ptr = 0;
  return n;
}

ssize_t
Readline(int fd, void *ptr, size_t len) {
  ssize_t res;

  if ((res = readline(fd, ptr, len)) == -1) {
    perror("readline");
    exit(1);
  }
  return res;
}