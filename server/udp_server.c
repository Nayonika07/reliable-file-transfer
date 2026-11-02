#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

#define BUFFER_SIZE 1400
#define DATA_SIZE (BUFFER_SIZE - 8)

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
        fprintf(stderr,
                "Usage: %s <server_port> <output_file>\n",
                argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    const char *filename = argv[2];

    if (port < 1 || port > 65535) {
        fprintf(stderr, "Invalid port.\n");
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {0};
    struct sockaddr_in client = {0};

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

    unsigned long received_packets = 0;
    unsigned long acknowledged_packets = 0;
    unsigned long duplicate_packets = 0;
    unsigned long total_bytes = 0;

    int transfer_complete = 0;

    printf("\n========== UDP RECEIVER ==========\n");
    printf("Listening on UDP port: %d\n", port);
    printf("Saving file as: %s\n", filename);
    printf("Waiting for sender...\n\n");
    fflush(stdout);

    while (!transfer_complete) {
        Packet packet = {0};
        client_len = sizeof(client);

        ssize_t n = recvfrom(
            sockfd, &packet, sizeof(packet), 0,
            (struct sockaddr *)&client, &client_len
        );

        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        if ((size_t)n < 8) {
            printf("[INVALID] Packet shorter than header. Ignored.\n");
            continue;
        }

        uint32_t seq = ntohl(packet.seq);
        uint32_t len = ntohl(packet.len);

        if (len > DATA_SIZE || (size_t)n != 8 + len) {
            printf("[INVALID] Packet %u has invalid length. Ignored.\n",
                   seq);
            continue;
        }

        if (seq == expected_seq) {
            if (len > 0) {
                size_t written = fwrite(packet.data, 1, len, file);

                if (written != len) {
                    perror("fwrite");
                    fclose(file);
                    close(sockfd);
                    return 1;
                }

                total_bytes += len;
                received_packets++;

                printf("[RX] Packet %u received | %u bytes\n", seq, len);
            } else {
                printf("[RX] EOF packet received | sequence %u\n", seq);
            }

            expected_seq++;
        } else if (expected_seq > 0 && seq == expected_seq - 1) {
            duplicate_packets++;
            printf("[DUPLICATE] Packet %u received again; not written twice\n",
                   seq);
        } else {
            printf("[OUT OF ORDER] Received %u; expected %u. Ignored.\n",
                   seq, expected_seq);
            continue;
        }

        fflush(file);

        Ack ack;
        ack.ack = htonl(seq);

        ssize_t ack_sent = sendto(
            sockfd, &ack, sizeof(ack), 0,
            (struct sockaddr *)&client, client_len
        );

        if (ack_sent != (ssize_t)sizeof(ack)) {
            perror("sendto ACK");
            continue;
        }

        acknowledged_packets++;
        printf("[ACK TX] ACK %u sent to sender\n", seq);
        fflush(stdout);

        if (len == 0) {
            transfer_complete = 1;
            printf("\n[COMPLETE] EOF acknowledged. Transfer finished.\n");
        }
    }

    fclose(file);
    close(sockfd);

    printf("\n========== RECEIVER REPORT ==========\n");
    printf("Output file: %s\n", filename);
    printf("Data packets received: %lu\n", received_packets);
    printf("ACKs sent: %lu\n", acknowledged_packets);
    printf("Duplicate packets: %lu\n", duplicate_packets);
    printf("Data written: %lu bytes\n", total_bytes);
    printf("RECEIVER STATUS: SUCCESS\n");
    printf("=====================================\n");

    return 0;
}
