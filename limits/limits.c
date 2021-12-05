#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#define printer(name) print_resource(#name, name)
void print_resource(char *, int);

void
main(int argc, char **argv) {
#ifdef RLIMIT_NOFILE
  printer(RLIMIT_NOFILE);
#endif
}

void
print_resource(char *name, int resource) {
  struct rlimit limit;
  u_int64_t lim;

  if (getrlimit(resource, &limit) == -1) {
    printf("%s getrlimit error\n", name);
  } else {
    if (limit.rlim_cur == RLIM_INFINITY) {
      printf("%s rlim_cur = (infinity);\n", name);
    } else {
      lim = limit.rlim_cur;
      printf("%s rlim_cur = %15ld\n", name, lim);
    }

    if (limit.rlim_max == RLIM_INFINITY) {
      printf("%s rlim_max = (infinity)\n", name);
    } else {
      lim = limit.rlim_max;
      printf("%s rlim_max = %15ld\n", name, lim);
    }
  }
}