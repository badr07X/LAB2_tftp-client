#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


#define TFTP_PORT 1069
#define BUFFER_SIZE 516       // 512 bytes data + 4 bytes header
#define RRQ_OPCODE 1
#define DATA_OPCODE 3
#define ACK_OPCODE 4
#define ERROR_OPCODE 5


int create_socket() {   
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

int create_rrq(char *buffer, const char *filename, const char *mode){
    buffer[0] = 0;
    buffer[1] = RRQ_OPCODE;          // Opcode for RRQ
    strcpy(buffer + 2, filename);    // Filename
    strcpy(buffer + 2 + strlen(filename) + 1, mode); // Mode ("octet")
    return 2 + strlen(filename) + 1 + strlen(mode) + 1;
}


// Function to send an ACK packet
void send_ack(int sockfd, struct sockaddr *server_addr, socklen_t server_addr_len, unsigned short block_number){
     char ack[4];
    ack[0] = 0;
    ack[1] = ACK_OPCODE;                 // Opcode for ACK
    ack[2] = (block_number >> 8) & 0xFF; // High byte of block number
    ack[3] = block_number & 0xFF;        // Low byte of block number

    if (sendto(sockfd, ack, sizeof(ack), 0, server_addr, server_addr_len) == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    printf("ACK sent for block: %d\n", block_number);

}

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
    int sockfd;
    unsigned short block_number = 0;
    ssize_t recv_len;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // Support IPv4 and IPv6
    hints.ai_socktype = SOCK_DGRAM;  // Use UDP for TFTP

    // Resolve the host address
    if ((status = getaddrinfo(host, NULL, &hints, &res)) != 0) {
        perror("getaddrinfo");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    memcpy(&server_addr, res->ai_addr, res->ai_addrlen);
    server_addr.sin_port = htons(TFTP_PORT);  // Set TFTP port

    freeaddrinfo(res);

    //Create socket

    sockfd = create_socket();

    // Build and send RRQ packet
    int rrq_len = create_rrq(buffer, filename, "octet");
    if (sendto(sockfd, buffer, rrq_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("RRQ sent for file: %s\n", filename);

    // Open file to save the received data
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("fopen");
        close(sockfd);
        return EXIT_FAILURE;
    }

    struct sockaddr_in from_addr;
    socklen_t from_addr_len = sizeof(from_addr);

      // Receive data packets and send ACKs
    while (1) {
        recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&from_addr, &from_addr_len);
        if (recv_len == -1) {
            perror("recvfrom");
            break;
        }

         // Check for DATA packet
        if (buffer[1] == DATA_OPCODE) {
            block_number = (buffer[2] << 8) | buffer[3];
            printf("Received block: %d\n", block_number);

            // Write data to file
            fwrite(buffer + 4, 1, recv_len - 4, file);

            // Send ACK for the received block
            send_ack(sockfd, (struct sockaddr *)&from_addr, from_addr_len, block_number);

            // If the data block is less than 512 bytes, it's the last block
            if (recv_len < BUFFER_SIZE) {
                printf("Last block received. File transfer complete.\n");
                break;
            }

        }

        else if (buffer[1] == ERROR_OPCODE) {
            printf("Error received: %s\n", buffer + 4);
            break;
        }
    }
    
    fclose(file);
    close(sockfd);
    return EXIT_SUCCESS;
        
}