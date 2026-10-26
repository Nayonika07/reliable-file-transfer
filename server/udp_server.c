#include <stdio.h>      // File handling and printing
#include <stdlib.h>     // atoi()
#include <string.h>     // memset(), strlen(), memcmp()
#include <unistd.h>     // close()
#include <arpa/inet.h>  // IPv4 address tools and htons()
#include <sys/socket.h> // socket(), bind(), recvfrom()

#define BUFFER_SIZE 1400 // Maximum bytes per datagram
#define EOF_MARKER "EOF" // Marks the end of the file

int main(int argc, char *argv[]) // Program entry point
{
    if (argc != 3) // Expect port and output filename
    {
        printf("Usage: %s <server_port> <output_file>\n", argv[0]); // Show usage
        return 1; // Exit with error
    }

    int server_port = atoi(argv[1]); // Convert port to integer
    const char *filename = argv[2]; // Output filename

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create IPv4 UDP socket

    if (sockfd < 0) // Check socket creation failure
    {
        perror("Error creating socket"); // Print error
        return 1; // Exit with error
    }

    struct sockaddr_in server_addr; // Store server address
    memset(&server_addr, 0, sizeof(server_addr)); // Clear address structure

    server_addr.sin_family = AF_INET; // Use IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all local IPv4 interfaces
    server_addr.sin_port = htons(server_port); // Convert port to network byte order

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) // Bind socket to port
    {
        perror("Error binding socket"); // Print error
        close(sockfd); // Close socket
        return 1; // Exit with error
    }

    FILE *file = fopen(filename, "wb"); // Open output file for binary writing

    if (file == NULL) // Check file opening failure
    {
        perror("Error opening output file"); // Print error
        close(sockfd); // Close socket
        return 1; // Exit with error
    }

    char buffer[BUFFER_SIZE]; // Hold one received chunk
    struct sockaddr_in client_addr; // Store sender's address
    socklen_t client_len = sizeof(client_addr); // Sender address structure size

    printf("Server listening on port %d...\n", server_port); // Show listening port
    printf("Saving received file as: %s\n", filename); // Show output filename

    while (1) // Receive datagrams until EOF or error
    {
        ssize_t bytes_received = recvfrom( // Receive one UDP datagram
            sockfd, // Socket receiving data
            buffer, // Store received bytes here
            BUFFER_SIZE, // Maximum bytes to receive
            0, // Default flags
            (struct sockaddr *)&client_addr, // Record sender address
            &client_len // Address structure size
        );

        if (bytes_received < 0) // Check receive error
        {
            perror("Error receiving data"); // Print error
            fclose(file); // Close output file
            close(sockfd); // Close socket
            return 1; // Exit with error
        }

        /* Check whether this datagram is the EOF marker. */
        if (bytes_received == strlen(EOF_MARKER) && // Must be exactly 3 bytes
            memcmp(buffer, EOF_MARKER, strlen(EOF_MARKER)) == 0) // Compare with "EOF"
        {
            printf("EOF marker received.\n"); // Report end marker
            break; // Stop receiving
        }

        fwrite(buffer, 1, bytes_received, file); // Save received bytes to file

        printf("Received %zd bytes\n", bytes_received); // Show chunk size
    }

    fclose(file); // Flush and close output file
    close(sockfd); // Release socket

    printf("File received successfully.\n"); // Report completion; no integrity check here

    return 0; // Exit successfully
}
