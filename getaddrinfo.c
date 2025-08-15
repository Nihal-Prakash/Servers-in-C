#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(void) {
    struct addrinfo hints;
    struct addrinfo *res;  // will point to the results
    int status;

    // Zero out the hints struct
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    // Example: get info for "www.example.com" port "80"
    status = getaddrinfo("www.example.com", "80", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    // Iterate through linked list of results
    struct addrinfo *p;
    for (p = res; p != NULL; p = p->ai_next) {
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        if (getnameinfo(p->ai_addr, p->ai_addrlen,
                        host, sizeof(host),
                        service, sizeof(service),
                        NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
            printf("Host: %s, Service: %s\n", host, service);
        }
    }

    freeaddrinfo(res); // free the linked list
    return 0;
}
