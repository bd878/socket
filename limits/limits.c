#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#define unavaliable(name) printf("%s resource unavailable\n", #name)

#define printer(name, desc) print_resource(#name, desc, name)
void print_resource(char *, char *, int);

void
main(int argc, char **argv) {
#ifdef RLIMIT_AS
  printer(RLIMIT_AS, "memory available to the process");
#elif
  unavaliable(RLIMIT_AS);
#endif

  printer(RLIMIT_CORE, "core dump file size");
  printer(RLIMIT_CPU, "process time available");
  printer(RLIMIT_DATA, "data segment available (init data, uninit data, heap)");
  printer(RLIMIT_FSIZE, "file size limit");
  printer(RLIMIT_NOFILE, "max open file descriptors");

#ifdef RLIMIT_MEMLOCK
  printer(RLIMIT_MEMLOCK, "max memory available for lock");
#elif
  unavaliable(RLIMIT_MEMLOCK);
#endif

#ifdef RLIMIT_MSGQUEUE
  printer(RLIMIT_MSGQUEUE, "max memory for POSIX messages queue");
#elif
  unavaliable(RLIMIT_MSGQUEUE);
#endif

#ifdef RLIMIT_NICE
  printer(RLIMIT_NICE, "max process priority");
#elif
  unavaliable(RLIMIT_NICE);
#endif

#ifdef RLIMIT_NPROC
  printer(RLIMIT_NPROC, "max child process amount");
#elif
  unavaliable(RLIMIT_NPROC);
#endif

#ifdef RLIMIT_RSS
  printer(RLIMIT_RSS, "virtual memory size limit");
#elif
  unavaliable(RLIMIT_RSS);
#endif

#ifdef RLIMIT_SIGPENDING
  printer(RLIMIT_SIGPENDING, "max signal queue size");
#elif
  unavaliable(RLIMIT_SIGPENDING);
#endif

  printer(RLIMIT_STACK, "stack size limit");

// #ifdef RLIMIT_NPTS
//   printer(RLIMIT_NPTS, "pseudoterminals limit");
// #elif
//   unavaliable(RLIMIT_NPTS);
// #endif

// #ifdef RLIMIT_SBSIZE
//   printer(RLIMIT_SBSIZE, "max sockets buffer limit");
// #elif
//   unavaliable(RLIMIT_SBSIZE);
// #endif

// #ifdef RLIMIT_SWAP
//   printer(RLIMIT_SWAP, "swap size limit");
// #elif
//   unavaliable(RLIMIT_SWAP);
// #endif
}

void
print_resource(char *name, char *desc, int resource) {
  struct rlimit limit;
  u_int64_t lim;

  if (getrlimit(resource, &limit) == -1) {
    printf("%s getrlimit error\n", name);
  } else {
    printf("%-20s ", name);

    if (limit.rlim_cur == RLIM_INFINITY) {
      printf("(бесконечность) ");
    } else {
      lim = limit.rlim_cur;
      printf("%15ld ", lim);
    }

    if (limit.rlim_max == RLIM_INFINITY) {
      printf("(бесконечность) ");
    } else {
      lim = limit.rlim_max;
      printf("%15ld ", lim);
    }
  }
  printf("%s\n", desc);
}