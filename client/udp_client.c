#include <stdio.h>      // File handling and printing
#include <stdlib.h>     // atoi()
#include <string.h>     // memset(), strlen()
#include <unistd.h>     // close()
#include <arpa/inet.h>  // IPv4 conversion and htons()
#include <sys/socket.h> // socket() and sendto()

#define BUFFER_SIZE 1400 // Maximum bytes per file chunk
#define EOF_MARKER "EOF" // Marks the end of the file

int main(int argc, char *argv[]) // Program entry point
{
    if (argc != 4) // Expect IP, port, and filename
    {
        printf("Usage: %s <server_ip> <server_port> <file>\n", argv[0]); // Show usage
        return 1; // Exit with error
    }

    const char *server_ip = argv[1]; // Server IP address
    int server_port = atoi(argv[2]); // Convert port to integer
    const char *filename = argv[3]; // File to send

    FILE *file = fopen(filename, "rb"); // Open file for binary reading

    if (file == NULL) // Check if file opening failed
    {
        perror("Error opening file"); // Print error reason
        return 1; // Stop the program
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create IPv4 UDP socket

    if (sockfd < 0) // Check socket creation failure
    {
        perror("Error creating socket"); // Print error
        fclose(file); // Close the file
        return 1; // Exit with error
    }

    struct sockaddr_in server_addr; // Store server address
    memset(&server_addr, 0, sizeof(server_addr)); // Clear address structure

    server_addr.sin_family = AF_INET; // Use IPv4
    server_addr.sin_port = htons(server_port); // Convert port to network byte order

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) // Convert IP to binary
    {
        perror("Invalid server IP address"); // Print error
        fclose(file); // Close file
        close(sockfd); // Close socket
        return 1; // Exit with error
    }

    char buffer[BUFFER_SIZE]; // Hold one file chunk
    size_t bytes_read; // Number of bytes read

    printf("Sending file: %s\n", filename); // Show filename
    printf("Server: %s:%d\n", server_ip, server_port); // Show destination

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) // Read file in chunks
    {
        ssize_t bytes_sent = sendto( // Send one UDP datagram
            sockfd, // Socket to use
            buffer, // Data to send
            bytes_read, // Number of bytes read
            0, // Default flags
            (struct sockaddr *)&server_addr, // Destination address
            sizeof(server_addr) // Address structure size
        );

        if (bytes_sent < 0) // Check sending error
        {
            perror("Error sending data"); // Print error
            fclose(file); // Close file
            close(sockfd); // Close socket
            return 1; // Exit with error
        }

        printf("Sent %zd bytes\n", bytes_sent); // Show bytes sent
    }

    /* Send marker to tell server the file has ended. */
    ssize_t bytes_sent = sendto( // Send EOF marker
        sockfd, // Socket to use
        EOF_MARKER, // "EOF" message
        strlen(EOF_MARKER), // Send 3 bytes
        0, // Default flags
        (struct sockaddr *)&server_addr, // Destination address
        sizeof(server_addr) // Address structure size
    );

    if (bytes_sent < 0) // Check EOF send error
    {
        perror("Error sending EOF marker"); // Print error
        fclose(file); // Close file
        close(sockfd); // Close socket
        return 1; // Exit with error
    }

    printf("EOF marker sent.\n"); // Confirm marker was sent
    printf("File sent successfully.\n"); // Reports sending, not confirmed receipt

    fclose(file); // Close input file
    close(sockfd); // Release socket

    return 0; // Exit successfully
}
