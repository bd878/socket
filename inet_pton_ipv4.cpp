#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cstring>

using namespace std;

int inet_pton_ipv4(int sock_family, const char *strptr, void *addrptr) {
  if (sock_family == AF_INET) {
    struct in_addr in_val;

    if (inet_aton(strptr, &in_val)) {
      cout << "sizeof(struct in_addr): " << sizeof(struct in_addr) << '\n';
      memcpy(addrptr, &in_val, sizeof(struct in_addr));
      return 1;
    }

    return 0;
  }

  errno = EAFNOSUPPORT;
  return -1;
}

int main() {
  char p_addr[] = "127.0.0.1";
  void *n_addr;
  cout << "sizeof(n_addr) before: " << sizeof(n_addr) << '\n';
  int result = inet_pton_ipv4(AF_INET, p_addr, n_addr);
  cout << "sizeof(n_addr) after: " << sizeof(n_addr) << '\n';
  if (result < 0) {
    cout << "inet_pton_ipv4() failed: " << strerror(errno) << '\n';
    exit(1);
  } else {
    for (int i = 0; i < sizeof(n_addr); ++i) {
      printf("%02x\n", ((uint8_t *)n_addr)[i]);
    }
  }
}
