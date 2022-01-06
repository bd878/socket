#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

void
daemonize(const char *cmd)
{
  int i, fd0, fd1, fd2;
  pid_t pid;
  struct rlimit rl;
  struct sigaction sa;

  umask(0);

  if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
    perror("no max descriptor");
  }

  if ((pid = fork()) < 0) {
    perror("error calling fork");
  } else if (pid != 0) {
    exit(0);
  }
  setsid();

  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0) {
    perror("cannot ignore sighup");
  }
  if ((pid = fork()) < 0) {
    perror("error calling fork");
  } else if (pid != 0) { /* parent process */
    exit(0);
  }

  if (chdir("/") < 0) {
    perror("cannot set working directory /");
  }

  if (rl.rlim_max == RLIM_INFINITY) { /* close all fds */
    rl.rlim_max = 1024;
  }
  for (i = 0; i < rl.rlim_max; i++) {
    close(i);
  }

  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  openlog(cmd, LOG_CONS, LOG_DAEMON);
  if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    syslog(LOG_ERR, "false fds %d %d %d", fd0, fd1, fd2);
    exit(1);
  }
}