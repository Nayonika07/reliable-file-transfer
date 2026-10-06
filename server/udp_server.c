#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1400
#define EOF_MARKER "EOF"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <server_port> <output_file>\n", argv[0]);
        return 1;
    }

    int server_port = atoi(argv[1]);
    const char *filename = argv[2];

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error binding socket");
        close(sockfd);
        return 1;
    }

    FILE *file = fopen(filename, "wb");

    if (file == NULL)
    {
        perror("Error opening output file");
        close(sockfd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    printf("Server listening on port %d...\n", server_port);
    printf("Saving received file as: %s\n", filename);

    while (1)
    {
        ssize_t bytes_received = recvfrom(
            sockfd,
            buffer,
            BUFFER_SIZE,
            0,
            (struct sockaddr *)&client_addr,
            &client_len
        );

        if (bytes_received < 0)
        {
            perror("Error receiving data");
            fclose(file);
            close(sockfd);
            return 1;
        }

        /* Check for EOF marker */
        if (bytes_received == strlen(EOF_MARKER) &&
            memcmp(buffer, EOF_MARKER, strlen(EOF_MARKER)) == 0)
        {
            printf("EOF marker received.\n");
            break;
        }

        fwrite(buffer, 1, bytes_received, file);

        printf("Received %zd bytes\n", bytes_received);
    }

    fclose(file);
    close(sockfd);

    printf("File received successfully.\n");

    return 0;
}
