
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 1024

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
    signal(SIGPIPE, SIG_IGN);

    int sock;
    struct sockaddr_in addr;
    char msg[SIZE];

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

    printf("Connected to server!\n");
    printf("Type exit to quit.\n");

    while (1) {
        printf("Enter message: ");
        fflush(stdout);

        if (fgets(msg, SIZE, stdin) == NULL)
            break;

        msg[strcspn(msg, "\n")] = '\0';

        if (sendLine(sock, msg) < 0) {
            perror("Send");
            break;
        }

        if (strcmp(msg, "exit") == 0)
            break;

        if (recvLine(sock, msg, SIZE) <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        printf("Server: %s\n", msg);

        if (strcmp(msg, "exit") == 0)
            break;
    }

    close(sock);
    return 0;
}


// gcc client.c -o client
// ./client
