#include <stdio.h>          // printf, perror
#include <stdlib.h>         // exit, free
#include <string.h>         // memset, strerror
#include <sys/types.h>      // basic data types
#include <sys/socket.h>     // socket(), sockaddr, AF_INET, etc.
#include <netdb.h>          // getaddrinfo(), struct addrinfo
#include <unistd.h>         // close()

typedef struct {
    int ai_flags;
    int ai_family;
    int ai_socketype;
    int ai_protocol;

    size_t ai_addrlen;

    struct sockaddr *ai_addr;

    char *ai_canonname;

    struct addrinfo *ai_next; //Linked list, next node
} addrinfo;

typedef struct {
    unsigned short sa_family;
    char           sa_data[14];
} sockaddr;

typedef struct {
    short int            sin_family; //"AF_INET"
    unsigned short int   sin_port;   //htons()
    struct in_addr       sin_addr;
    unsigned char        sin_zero[8]; //to be set to all 0's with memset()
} sockaddr_in;

//=================================================================================================//
// IPv6 structs
//=================================================================================================//

// (IPv6 only--see struct sockaddr_in and struct in_addr for IPv4)

struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};

struct in6_addr {
    unsigned char   s6_addr[16];   // IPv6 address
};

struct sockaddr_storage {
    sa_family_t  ss_family;     // address family

    // all this is padding, implementation specific, ignore it:
    char      __ss_pad1[_SS_PAD1SIZE];
    int64_t   __ss_align;
    char      __ss_pad2[_SS_PAD2SIZE];
};