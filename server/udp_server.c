
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define BUFFER_SIZE 1400
#define DATA_SIZE (BUFFER_SIZE - 8)
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
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_port> <output_file>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    const char *filename = argv[2];

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct timeval timeout = {TIMEOUT_SEC, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    struct sockaddr_in addr = {0}, client = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("fopen");
        close(sockfd);
        return 1;
    }

    uint32_t expected_seq = 0;
    socklen_t client_len = sizeof(client);

    printf("Server listening on port %d\n", port);
    printf("Saving received file as: %s\n", filename);

    for (;;) {
        Packet packet;
        ssize_t n = recvfrom(sockfd, &packet, sizeof(packet), 0,
                             (struct sockaddr *)&client, &client_len);

        if (n < 0) {
            continue;
        }

        if ((size_t)n < 8) {
            fprintf(stderr, "Invalid packet: too short.\n");
            continue;
        }

        uint32_t seq = ntohl(packet.seq);
        uint32_t len = ntohl(packet.len);

        if (len > DATA_SIZE || (size_t)n != 8 + len) {
            fprintf(stderr, "Invalid packet received; ignored.\n");
            continue;
        }

        if (seq == expected_seq) {
            if (len > 0 && fwrite(packet.data, 1, len, file) != len) {
                perror("fwrite");
                fclose(file);
                close(sockfd);
                return 1;
            }

            printf("Received packet %u (%u bytes)\n", seq, len);
            expected_seq++;
        } else if (expected_seq > 0 && seq == expected_seq - 1) {
            printf("Duplicate packet %u; not writing again\n", seq);
        } else {
            printf("Unexpected packet %u; expected %u\n", seq, expected_seq);
            continue;
        }

        Ack ack;
        ack.ack = htonl(seq);

        if (sendto(sockfd, &ack, sizeof(ack), 0,
                   (struct sockaddr *)&client, client_len) < 0) {
            perror("sendto ACK");
            continue;
        }

        if (len == 0) {
            printf("End-of-file packet acknowledged.\n");
            break;
        }
    }

    fclose(file);
    close(sockfd);
    printf("File received successfully.\n");
    return 0;
}
