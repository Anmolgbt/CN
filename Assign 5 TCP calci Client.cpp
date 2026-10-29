
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 100

int sendLine(int fd, const char *msg) {
    char data[SIZE];
    int len = snprintf(data, sizeof(data), "%s\n", msg);

    if (len < 0 || len >= SIZE) return -1;

    int sent = 0;
    while (sent < len) {
        int n = send(fd, data + sent, len - sent, 0);
        if (n <= 0) return -1;
        sent += n;
    }
    return 0;
}

int recvLine(int fd, char *buf, int size) {
    int i = 0;
    char ch;

    while (i < size - 1) {
        int n = recv(fd, &ch, 1, 0);
        if (n <= 0) return n;

        if (ch == '\n') break;
        buf[i++] = ch;
    }

    buf[i] = '\0';
    return 1;
}

int main() {
    int sock;
    struct sockaddr_in addr;
    long long a, b;
    char op, buffer[SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&addr,
                sizeof(addr)) < 0) {
        perror("Connect");
        close(sock);
        return 1;
    }

    printf("Enter first number: ");
    if (scanf("%lld", &a) != 1) {
        printf("Invalid number.\n");
        close(sock);
        return 1;
    }

    printf("Enter operator (+, -, *, /): ");
    if (scanf(" %c", &op) != 1) {
        printf("Invalid operator.\n");
        close(sock);
        return 1;
    }

    printf("Enter second number: ");
    if (scanf("%lld", &b) != 1) {
        printf("Invalid number.\n");
        close(sock);
        return 1;
    }

    snprintf(buffer, SIZE, "%lld %c %lld", a, op, b);

    if (sendLine(sock, buffer) < 0) {
        perror("Send");
        close(sock);
        return 1;
    }

    if (recvLine(sock, buffer, SIZE) <= 0) {
        printf("Failed to receive result.\n");
        close(sock);
        return 1;
    }

    printf("Result from Server: %s\n", buffer);

    close(sock);
    return 0;
}

// gcc calc_client.c -o calc_client
// ./calc_client
