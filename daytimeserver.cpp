#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <iostream>

using namespace std;

int BUF_LEN = 1000;

int main(int argc, char **argv) {
  int listener, sock;
  struct sockaddr_in addr;
  char buff[BUF_LEN];
  time_t ticks;

  listener = socket(AF_INET, SOCK_STREAM, 0);
  if (listener < 0) {
    throw runtime_error("cannot listen");
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(13);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(2);
  }

  listen(listener, 1);

  cout << "Waiting for connections..." << '\n';
  while (1) {
    sock = accept(listener, NULL, NULL);
    cout << "Connection received!" << '\n';

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\er\en", ctime(&ticks));
    send(sock, buff, strlen(buff), 0);

    close(sock);
    cout << "Connection closed" << '\n';
  }

  return 0;
}