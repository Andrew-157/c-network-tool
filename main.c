#define _GNU_SOURCE /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <sys/types.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>


struct interface {
	char *name;
	char *ipv4_addr;
	char *ipv6_addr;
};


int main() {
	struct ifaddrs *ifaddr;
	int family, s;
	int count = 0;
	char host[NI_MAXHOST];
	struct interface* interfaces;

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(errno);
	}

	for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		family = ifa->ifa_addr->sa_family;
		if (family == AF_PACKET) {
			continue;
		}

		struct interface curr;
		int interface_already_in_array = 0; 

		for (int i = 0; i < count; i++) {
			if (strcmp(interfaces[i].name, ifa->ifa_name) == 0) {
				curr = interfaces[i];
				interface_already_in_array = 1;
			    break;
			}
		}

		curr.name = ifa->ifa_name;
		s = getnameinfo(
			ifa->ifa_addr,
			(family == AF_INET) ?
			sizeof(struct sockaddr_in) :
			sizeof(struct sockaddr_in6),
			host,
			NI_MAXHOST,
			NULL,
			0,
			NI_NUMERICHOST	
		);

		if (family == AF_INET) {
			curr.ipv4_addr = host;
		} else {
			curr.ipv6_addr = host;
		}

		if (s != 0) {
			fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(s));
			exit(errno);
		}

		if (!interface_already_in_array) {
			interfaces = (struct interface *)realloc(interfaces, (count + 1) * sizeof(struct interface));
			if (interfaces == NULL) {
				fprintf(stderr, "Failed to allocate memory\n");
				exit(errno);	
			}

			interfaces[count++] = curr;
		}

		printf("DEBUG: %s %s %s\n", curr.name, curr.ipv4_addr, curr.ipv6_addr);

	}

	for (int i = 0; i < count; i++) {
		printf("%d. %s\n", (i + 1), interfaces[i].name);
		if (interfaces[i].ipv4_addr != NULL) {
			printf("\tinet4: %s\n", interfaces[i].ipv4_addr);
		}
		if (interfaces[i].ipv6_addr != NULL) {
			printf("\tinet6: %s\n", interfaces[i].ipv6_addr);
		}
	}

	free(interfaces);
	freeifaddrs(ifaddr);
	exit(0);
}
