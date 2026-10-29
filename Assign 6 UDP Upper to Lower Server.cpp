
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
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server,
             sizeof(server)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    printf("UDP Server started...\n");
    printf("Waiting for client...\n\n");

    // 1. Receive Hello
    n = recvfrom(sockfd, buffer, SIZE - 1, 0,
                 (struct sockaddr *)&client, &len);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    // Send Hello
    char *msg = "Hello!";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&client, len);
    printf("Server: %s\n", msg);

    // 2. Receive How are you?
    n = recvfrom(sockfd, buffer, SIZE - 1, 0,
                 (struct sockaddr *)&client, &len);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    msg = "Fine.";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&client, len);
    printf("Server: %s\n", msg);

    // 3. Receive CN lab question
    n = recvfrom(sockfd, buffer, SIZE - 1, 0,
                 (struct sockaddr *)&client, &len);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    msg = "Yes.";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&client, len);
    printf("Server: %s\n", msg);

    // 4. Receive attending question
    n = recvfrom(sockfd, buffer, SIZE - 1, 0,
                 (struct sockaddr *)&client, &len);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    msg = "Yes.";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&client, len);
    printf("Server: %s\n", msg);

    // 5. Ask final question
    msg = "Why should we chat?";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&client, len);
    printf("Server: %s\n", msg);

    // 6. Receive client's answer
    n = recvfrom(sockfd, buffer, SIZE - 1, 0,
                 (struct sockaddr *)&client, &len);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }
    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    // 7. End conversation
    msg = "Bye!";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&client, len);
    printf("Server: %s\n", msg);

    printf("\nChat ended by server.\n");

    close(sockfd);
    return 0;
}

// gcc server.c -o server
// ./server
