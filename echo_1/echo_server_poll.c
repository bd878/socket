#include "../lib.h"
#include <limits.h>

int main() {
  const int SOCKFD_I = 0;
  const int OPEN_MAX = 100;
  static const int MAXLINE = 1024;
  char buf[MAXLINE];
  struct sockaddr_in addr, caddr;
  int sockfd, clifd;
  struct pollfd fds[OPEN_MAX];
  int ccount;
  int nready;
  int nread;
  socklen_t clilen;

  sockfd = Socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5436);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  Bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  Listen(sockfd, 5);

  for (int i = 0; i < OPEN_MAX; ++i) {
    fds[i].fd = -1;
  }

  nread = 0;
  nready = 0;
  ccount = 0;
  fds[SOCKFD_I].fd = sockfd;
  fds[SOCKFD_I].events = POLLRDNORM;

  while (1) {
    nready = Poll(fds, ccount + 1, INFTIM);

    if (fds[SOCKFD_I].revents & POLLRDNORM) { /* new client */
      clilen = sizeof(caddr);
      clifd = Accept(sockfd, (struct sockaddr *)&caddr, &clilen);

      int free_fd_i = -1;
      for (int i = 1; i < OPEN_MAX && free_fd_i == -1; ++i) {
        if (fds[i].fd == -1) {
          fds[i].fd = clifd;
          fds[i].events = POLLRDNORM;
          free_fd_i = i;
        }
      }

      if (free_fd_i == -1) {
        printf("Too many clients\n");
      } else {
        if (free_fd_i > ccount) {
          ccount += 1;
        }

        if (nready > 0) {
          nready -= 1;
        }
      }
    }

    for (int i = 1; i <= ccount && nready > 0; i++) { /* new data or error */
      if (fds[i].revents & (POLLRDNORM | POLLERR)) {
        if ((nread = Read(fds[i].fd, buf, MAXLINE)) < 0) {
          if (errno == ECONNRESET) {
            Close(fds[i].fd);
            fds[i].fd = -1;
          } else {
            perror("read error");
          }
        } else if (nread == 0) {
          Close(fds[i].fd);
          fds[i].fd = -1;
        } else {
          Writen(fds[i].fd, buf, nread);
        }

        nready -= 1;
      }
    }
  }
}