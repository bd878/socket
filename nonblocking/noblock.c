#include "../lib.h"

char buf[500000];

// ./noblock.out < /etc/services > temp.file
// ./noblock.out < /etc/services 2>stderr.out
int main() {
  ssize_t nwritten, nreaden;
  char *ptr;

  nreaden = read(STDIN_FILENO, buf, sizeof(buf));
  fprintf(stderr, "readen %ld bytes\n", nreaden);

  Setfl(STDOUT_FILENO, O_NONBLOCK);
  ptr = buf;
  while (nreaden > 0) { // polling
    errno = 0;
    nwritten = write(STDOUT_FILENO, buf, nreaden);
    fprintf(stderr, "bytes written = %ld; error = %d\n", nwritten, errno);

    if (nwritten > 0) {
      ptr += nwritten;
      nreaden -= nwritten;
    }
  }

  Clrfl(STDOUT_FILENO, O_NONBLOCK);

  return 0;
}