#include <sys/types.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>


int main() {
    struct ifaddrs *ifaddr;
    int family;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        family = ifa->ifa_addr->sa_family;
        printf("%s\n", ifa->ifa_name);
    }

    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}
