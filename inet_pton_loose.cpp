#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// int inet_pton_loosen(int family, const char *src, void *dst) {
//   if (family == AF_INET) {
//     int pton_ipv4 = inet_pton(family, src, dst);
//     if (pton_ipv4 == 0) {
//       struct in_addr* addr;
//       if (inet_aton(src, addr)) {
//         memcpy(dst, addr->s_addr);
//       } else {
//         return -1;
//       }
//     }
//   }
// }

int main() {
  const char ip[] = "127.0.0.1\0";

  uint32_t pton_bytes;
  int pton_ipv4 = inet_pton(AF_INET, ip, &pton_bytes);
  printf("%d\n", pton_ipv4);
  printf("%d\n", pton_bytes);

  uint32_t aton_bytes;
  struct in_addr addr;
  int aton_ipv4 = inet_aton(ip, &addr);
  printf("%ld\n", sizeof(addr.s_addr));
  memcpy(&aton_bytes, &addr.s_addr, sizeof(addr.s_addr));
  printf("%d\n", aton_ipv4);
  printf("%d\n", aton_bytes);

  return 0;
}