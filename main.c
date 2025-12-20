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
	struct interface *next;
};


int main() {
	struct ifaddrs *ifaddr;

	int count = 0;
	int family, s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(errno);
	}

	for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		family = ifa->ifa_addr->sa_family;
		if (family == AF_PACKET) {
			continue;
		}

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

		if (s != 0) {
			printf("getnameinfo() failed: %s\n", gai_strerror(s));
			exit(errno);
		}

		printf("%d. %s:\n\t%s\n", (count + 1), ifa->ifa_name, host);
		count++;
	}

	freeifaddrs(ifaddr);
	exit(0);
}
