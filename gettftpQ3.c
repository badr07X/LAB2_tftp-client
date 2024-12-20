#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    // Variables to store host and file
    char *host;
    char *file;

    // Check if arguments are passed
    if (argc >= 3) {
        host = argv[1];  // First argument: host
        file = argv[2];  // Second argument: file
    } else {
        // Use environment variables if arguments are missing
        host = getenv("TFTP_HOST");
        file = getenv("TFTP_FILE");

        if (!host || !file) {
            printf("Usage: %s <host> <file>\n", argv[0]);
            printf("Or set environment variables TFTP_HOST and TFTP_FILE.\n");
            return 1;
        }
    }

    // Display the base information
    printf("Resolving host: %s, File: %s\n", host, file);

    // Structure to store address information
    struct addrinfo hints, *res, *p;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // Support IPv4 and IPv6
    hints.ai_socktype = SOCK_DGRAM;  // Use UDP for TFTP

    // Resolve the host address
    if ((status = getaddrinfo(host, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    // Loop through the resolved addresses
    printf("Resolved addresses:\n");
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char ipstr[INET6_ADDRSTRLEN];

        // Get pointer to the address (IPv4 or IPv6)
        if (p->ai_family == AF_INET) {  // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {  // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        // Convert address to readable string
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("  %s\n", ipstr);
    }

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(res);

    return 0;  // Successful execution
}
