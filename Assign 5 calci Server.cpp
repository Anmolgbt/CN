
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
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
    signal(SIGPIPE, SIG_IGN);

    int server, client;
    struct sockaddr_in addr;
    char buffer[SIZE], reply[SIZE];
    long long a, b, result;
    char op;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(server, (struct sockaddr *)&addr,
             sizeof(addr)) < 0) {
        perror("Bind");
        close(server);
        return 1;
    }

    if (listen(server, 5) < 0) {
        perror("Listen");
        close(server);
        return 1;
    }

    printf("Calculator server waiting on port %d...\n", PORT);

    client = accept(server, NULL, NULL);
    if (client < 0) {
        perror("Accept");
        close(server);
        return 1;
    }

    printf("Client connected!\n");

    if (recvLine(client, buffer, SIZE) <= 0) {
        printf("Failed to receive calculation.\n");
        close(client);
        close(server);
        return 1;
    }

    if (sscanf(buffer, "%lld %c %lld", &a, &op, &b) != 3) {
        strcpy(reply, "Invalid input");
    } else {
        switch (op) {
            case '+':
                result = a + b;
                snprintf(reply, SIZE, "%lld", result);
                break;

            case '-':
                result = a - b;
                snprintf(reply, SIZE, "%lld", result);
                break;

            case '*':
                result = a * b;
                snprintf(reply, SIZE, "%lld", result);
                break;

            case '/':
                if (b == 0) {
                    strcpy(reply, "Error: Division by zero");
                } else {
                    // Integer division
                    result = a / b;
                    snprintf(reply, SIZE, "%lld", result);
                }
                break;

            default:
                strcpy(reply, "Invalid operator");
        }

        printf("Calculation: %s = %s\n", buffer, reply);
    }

    if (sendLine(client, reply) < 0)
        perror("Send");

    close(client);
    close(server);
    return 0;
}

// gcc calc_server.c -o calc_server
// ./calc_server