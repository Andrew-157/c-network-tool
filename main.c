#include <sys/types.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    struct ifaddrs *ifaddr;
    //int family;

    char **names = NULL;
    int count = 0;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        //family = ifa->ifa_addr->sa_family;
        if (count == 0) {
            names = (char**)malloc(1 * sizeof(char *));
            if (names == NULL) {
                fprintf(stderr, "Failed to allocate memory\n");
                exit(1);
            }
            names[0] = ifa->ifa_name;
        } else {
            names = realloc(names, (count + 1) * sizeof(char *));
            names[count] = ifa->ifa_name;
        }
        count++;
    }

    for (int i = 0; i < count; i++) {
        printf("Ifname: %s\n", names[i]);
    }

    free(names);

    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}
