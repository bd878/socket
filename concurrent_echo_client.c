#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  pid_t proc_ids[5];

  for (int i = 0; i < 5; ++i) {
    if ((proc_ids[i] = fork()) == -1) {
      perror("fork");
      exit(1);
    }

    if (proc_ids[i] == 0) { /* child */
      pid_t proc_id = proc_ids[i];
      printf("running process %d under %d pid\n", i, proc_id);

      struct sockaddr_in addr;
      int sock;

      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(2);
      }

      addr.sin_family = AF_INET;
      addr.sin_port = htons(3425);
      addr.sin_addr.s_addr = htonl(INADDR_ANY);

      if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) > 0) {
        perror("connect");
        exit(2);
      }

      char message[100];
      snprintf(message, sizeof(message), "Proc %d sends: Hello, world\n", proc_id);
      char buff[sizeof(message)];
      send(sock, message, sizeof(message), 0);
      recv(sock, buff, sizeof(message), 0);

      printf("received in proc %d %s\n", i, buff);
      close(sock);

      printf("process %d has finished\n", i);
      exit(0);
    }

    exit(0);
  }
}