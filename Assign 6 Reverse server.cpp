
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
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket failed");
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

    n = recvfrom(sockfd, buffer, SIZE - 1, 0,
                 (struct sockaddr *)&client, &len);

    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Original String: %s\n", buffer);

    // Reverse the string
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    printf("Reversed String: %s\n", buffer);

    sendto(sockfd, buffer, n, 0,
           (struct sockaddr *)&client, len);

    close(sockfd);
    return 0;
}

// gcc reverse_server.c -o reverse_server
// ./reverse_server