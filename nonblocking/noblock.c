#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

char buf[500000];

// ./noblock.out < /etc/services > temp.file
int main() {
  ssize_t nwritten, nreaden;
  char *ptr;

  nreaden = read(STDIN_FILENO, buf, sizeof(buf));
  dprintf(STDERR_FILENO, "readen %ld bytes\n", nreaden);

  fcntl(STDOUT_FILENO, O_NONBLOCK);
  ptr = buf;
  while (nreaden > 0) {
    errno = 0;
    nwritten = write(STDOUT_FILENO, buf, nreaden);
    dprintf(STDERR_FILENO, "bytes written = %ld; error = %d\n", nwritten, errno);

    if (nwritten > 0) {
      ptr += nwritten;
      nreaden -= nwritten;
    }
  }

  return 0;
}