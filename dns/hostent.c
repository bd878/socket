#include "../lib.h"
#include <netdb.h>

int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Usage: ./a.out ...<AddressNames>\n");
    exit(1);
  }

  struct hostent *hptr;
  char **alias;
  char **addr;
  char addrstr[INET_ADDRSTRLEN];

  for (int i = 1; i < argc; ++i) {
    if ((hptr = gethostbyname(argv[i])) == NULL) {
      printf("gethostbyname error for host %s: %s\n",
        argv[i], hstrerror(h_errno));
    } else {
      printf("official name is %s\n", hptr->h_name);
      for (alias = hptr->h_aliases; *alias != NULL; ++alias) {
        printf("\talias is %s\n", *alias);
      }

      switch (hptr->h_addrtype) {
        case AF_INET:
          for (addr = hptr->h_addr_list; *addr != NULL; ++addr) {
            printf("\taddress: %s\n",
              inet_ntop(hptr->h_addrtype, *addr, addrstr, sizeof(addrstr)));
          }
        default:
          printf("unknown address type\n");
      }
    }
  }

  return 0;
}