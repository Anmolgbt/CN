
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define SIZE 1024

int main() {
    int sockfd;
    char buffer[SIZE];
    struct sockaddr_in server;
    socklen_t len = sizeof(server);
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // 1. Send Hello
    char *msg = "Hello!";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&server, len);
    printf("Client: %s\n", msg);

    n = recvfrom(sockfd, buffer, SIZE - 1, 0, NULL, NULL);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    // 2. Ask how are you?
    msg = "How are you?";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&server, len);
    printf("Client: %s\n", msg);

    n = recvfrom(sockfd, buffer, SIZE - 1, 0, NULL, NULL);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    // 3. Ask about CN lab
    msg = "Is today CN lab today?";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&server, len);
    printf("Client: %s\n", msg);

    n = recvfrom(sockfd, buffer, SIZE - 1, 0, NULL, NULL);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    // 4. Ask about attending
    msg = "Are you attending?";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&server, len);
    printf("Client: %s\n", msg);

    n = recvfrom(sockfd, buffer, SIZE - 1, 0, NULL, NULL);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    // 5. Receive final question
    n = recvfrom(sockfd, buffer, SIZE - 1, 0, NULL, NULL);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    // Send answer
    msg = "Because communication is important!";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&server, len);
    printf("Client: %s\n", msg);

    // 6. Receive Bye
    n = recvfrom(sockfd, buffer, SIZE - 1, 0, NULL, NULL);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    if (strcmp(buffer, "Bye!") == 0)
        printf("\nChat ended by server.\n");

    close(sockfd);
    return 0;
}

// gcc client.c -o client
// ./client