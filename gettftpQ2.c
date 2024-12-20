#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Variables to store host and file
    char *host;
    char *file;

    // Check if arguments are passed
    if (argc >= 3) {
        // Use command-line arguments
        host = argv[1];  // First argument: host
        file = argv[2];  // Second argument: file
    } else {
        // Otherwise, use environment variables
        host = getenv("TFTP_HOST");  // Retrieve TFTP_HOST from the environment
        file = getenv("TFTP_FILE");  // Retrieve TFTP_FILE from the environment

        // Check if the environment variables exist
        if (!host || !file) {
            printf("Usage: %s <host> <file>\n", argv[0]);
            printf("Or set environment variables TFTP_HOST and TFTP_FILE.\n");
            return 1;  // Return an error if no values are provided
        }
    }

    // Print the final values of the host and file variables
    printf("Host: %s, File: %s\n", host, file);
    return 0;  // Return 0 for successful execution
}
