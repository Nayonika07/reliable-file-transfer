#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define BUFFER_SIZE 1400
#define DATA_SIZE (BUFFER_SIZE - 8)
#define MAX_RETRIES 10
#define TIMEOUT_SEC 2

typedef struct {
    uint32_t seq;
    uint32_t len;
    char data[DATA_SIZE];
} Packet;

typedef struct {
    uint32_t ack;
} Ack;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port> <file>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    const char *filename = argv[3];

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        fclose(file);
        return 1;
    }

    struct timeval timeout = {TIMEOUT_SEC, 0};
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
                   &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        fclose(file);
        close(sockfd);
        return 1;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server.sin_addr) != 1) {
        fprintf(stderr, "Invalid server IP address: %s\n", server_ip);
        fclose(file);
        close(sockfd);
        return 1;
    }

    uint32_t seq = 0;
    char data[DATA_SIZE];

    printf("Sending file: %s\n", filename);

    for (;;) {
        size_t len = fread(data, 1, sizeof(data), file);

        if (ferror(file)) {
            perror("fread");
            fclose(file);
            close(sockfd);
            return 1;
        }

        Packet packet = {0};
        packet.seq = htonl(seq);
        packet.len = htonl((uint32_t)len);
        if (len > 0)
            memcpy(packet.data, data, len);

        size_t packet_size = 8 + len;
        int acknowledged = 0;

        for (int attempt = 1; attempt <= MAX_RETRIES; attempt++) {
            ssize_t sent = sendto(sockfd, &packet, packet_size, 0,
                                  (struct sockaddr *)&server, sizeof(server));

            if (sent != (ssize_t)packet_size) {
                perror("sendto");
                fclose(file);
                close(sockfd);
                return 1;
            }

            printf("Sent packet %u (%zu bytes), attempt %d\n",
                   seq, len, attempt);

            Ack ack;
            struct sockaddr_in from;
            socklen_t from_len = sizeof(from);

            ssize_t received = recvfrom(sockfd, &ack, sizeof(ack), 0,
                                        (struct sockaddr *)&from, &from_len);

            if (received == (ssize_t)sizeof(ack) &&
                from.sin_addr.s_addr == server.sin_addr.s_addr &&
                from.sin_port == server.sin_port &&
                ntohl(ack.ack) == seq) {
                printf("Received ACK %u\n", seq);
                acknowledged = 1;
                break;
            }

            if (received < 0)
                printf("ACK timeout; retrying packet %u\n", seq);
            else
                printf("Unexpected ACK; retrying packet %u\n", seq);
        }

        if (!acknowledged) {
            fprintf(stderr, "Failed: packet %u was not acknowledged.\n", seq);
            fclose(file);
            close(sockfd);
            return 1;
        }

        if (len == 0) {
            printf("Empty EOF packet acknowledged. Transfer complete.\n");
            break;
        }

        seq++;
    }

    fclose(file);
    close(sockfd);
    return 0;
}
