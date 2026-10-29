
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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
        perror("Socket failed");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    printf("Enter a string: ");
    if (fgets(buffer, SIZE, stdin) == NULL) {
        close(sockfd);
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server, len);

    n = recvfrom(sockfd, buffer, SIZE - 1, 0, NULL, NULL);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Reversed String from Server: %s\n", buffer);

    close(sockfd);
    return 0;
}

// gcc reverse_client.c -o reverse_client
// ./reverse_client
