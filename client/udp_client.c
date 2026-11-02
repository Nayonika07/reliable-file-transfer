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

static double elapsed_seconds(struct timespec start,
                              struct timespec end)
{
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s <server_ip> <server_port> <file>\n",
                argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    const char *filename = argv[3];

    if (port < 1 || port > 65535) {
        fprintf(stderr, "Invalid server port.\n");
        return 1;
    }

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

    unsigned long total_data_packets = 0;
    unsigned long total_transmissions = 0;
    unsigned long acknowledged_packets = 0;
    unsigned long retransmissions = 0;
    unsigned long failed_packets = 0;
    unsigned long total_bytes = 0;
    unsigned long eof_transmissions = 0;
    int eof_acknowledged = 0;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    printf("\n========== UDP FILE TRANSFER ==========\n");
    printf("File: %s\n", filename);
    printf("Destination IP: %s\n", server_ip);
    printf("Destination port: %d\n", port);
    printf("=======================================\n\n");
    fflush(stdout);

    for (;;) {
        size_t len = fread(data, 1, sizeof(data), file);

        if (ferror(file)) {
            perror("fread");
            fclose(file);
            close(sockfd);
            return 1;
        }

        int is_eof = (len == 0);

        Packet packet = {0};
        packet.seq = htonl(seq);
        packet.len = htonl((uint32_t)len);

        if (len > 0)
            memcpy(packet.data, data, len);

        size_t packet_size = 8 + len;
        int acknowledged = 0;

        if (!is_eof)
            total_data_packets++;

        for (int attempt = 1; attempt <= MAX_RETRIES; attempt++) {
            ssize_t sent = sendto(
                sockfd, &packet, packet_size, 0,
                (struct sockaddr *)&server, sizeof(server)
            );

            if (sent != (ssize_t)packet_size) {
                perror("sendto");
                fclose(file);
                close(sockfd);
                return 1;
            }

            total_transmissions++;

            if (is_eof)
                eof_transmissions++;

            if (attempt > 1)
                retransmissions++;

            printf("[TX] Packet %u | %zu bytes | attempt %d/%d\n",
                   seq, len, attempt, MAX_RETRIES);
            fflush(stdout);

            Ack ack = {0};
            struct sockaddr_in from = {0};
            socklen_t from_len = sizeof(from);

            ssize_t received = recvfrom(
                sockfd, &ack, sizeof(ack), 0,
                (struct sockaddr *)&from, &from_len
            );

            if (received == (ssize_t)sizeof(ack) &&
                from.sin_addr.s_addr == server.sin_addr.s_addr &&
                from.sin_port == server.sin_port &&
                ntohl(ack.ack) == seq) {

                printf("[ACK] Packet %u acknowledged by receiver\n", seq);
                fflush(stdout);

                acknowledged = 1;
                acknowledged_packets++;

                if (is_eof)
                    eof_acknowledged = 1;

                break;
            }

            if (received < 0) {
                printf("[TIMEOUT] No ACK for packet %u. Retrying...\n",
                       seq);
            } else {
                printf("[WARNING] Invalid or unexpected ACK for packet %u\n",
                       seq);
            }

            fflush(stdout);
        }

        if (!acknowledged) {
            fprintf(stderr,
                    "[FAILED] Packet %u was not acknowledged after %d attempts.\n",
                    seq, MAX_RETRIES);

            failed_packets++;
            break;
        }

        if (is_eof)
            break;

        total_bytes += len;
        seq++;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double duration = elapsed_seconds(start_time, end_time);

    fclose(file);
    close(sockfd);

    printf("\n========== TRANSFER REPORT ==========\n");
    printf("File: %s\n", filename);
    printf("Data packets: %lu\n", total_data_packets);
    printf("Total transmissions: %lu\n", total_transmissions);
    printf("Acknowledged packets: %lu\n", acknowledged_packets);
    printf("Retransmissions: %lu\n", retransmissions);
    printf("Failed packets: %lu\n", failed_packets);
    printf("Data transferred: %lu bytes\n", total_bytes);
    printf("EOF transmissions: %lu\n", eof_transmissions);
    printf("EOF acknowledged: %s\n",
           eof_acknowledged ? "YES" : "NO");
    printf("Transfer duration: %.3f seconds\n", duration);

    if (failed_packets == 0 && eof_acknowledged) {
        printf("TRANSFER STATUS: SUCCESS\n");
    } else {
        printf("TRANSFER STATUS: FAILED\n");
    }

    printf("=====================================\n");

    return (failed_packets == 0 && eof_acknowledged) ? 0 : 1;
}
