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
    if (argc != 4)
    {
        printf("Usage: %s <server_ip> <server_port> <file>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    const char *filename = argv[3];

    FILE *file = fopen(filename, "rb");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("Error creating socket");
        fclose(file);
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid server IP address");
        fclose(file);
        close(sockfd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    printf("Sending file: %s\n", filename);
    printf("Server: %s:%d\n", server_ip, server_port);

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        ssize_t bytes_sent = sendto(
            sockfd,
            buffer,
            bytes_read,
            0,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)
        );

        if (bytes_sent < 0)
        {
            perror("Error sending data");
            fclose(file);
            close(sockfd);
            return 1;
        }

        printf("Sent %zd bytes\n", bytes_sent);
    }

    /* Send EOF marker to indicate end of file */
    ssize_t bytes_sent = sendto(
        sockfd,
        EOF_MARKER,
        strlen(EOF_MARKER),
        0,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr)
    );

    if (bytes_sent < 0)
    {
        perror("Error sending EOF marker");
        fclose(file);
        close(sockfd);
        return 1;
    }

    printf("EOF marker sent.\n");
    printf("File sent successfully.\n");

    fclose(file);
    close(sockfd);

    return 0;
}
