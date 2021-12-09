#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

#define OPEN_MAX_GUESS 256

long
get_open_max(void) {
  if (openmax == 0) {
    errno = 0;
    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
      if (errno == 0) {
        openmax = OPEN_MAX_GUESS;
      } else {
        perror("sysconf _SC_OPEN_MAX");
      }
    }
  }
  return openmax;  
}

int
main() {
  printf("MAX open fds available: %ld\n", get_open_max());
  return 0;
}
