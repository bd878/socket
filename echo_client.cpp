#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main() {
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    cout << "socket" << endl;
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) > 0) {
    cout << "connect" << endl;
    exit(2);
  }

  char message[] = "Hello there!\n";
  char buf[sizeof(message)];
  send(sock, message, sizeof(message), 0);
  recv(sock, buf, sizeof(message), 0);

  cout << buf << endl;
  close(sock);

  return 0;
}