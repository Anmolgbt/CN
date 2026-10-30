
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in a{};

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);

    if (connect(s, (sockaddr*)&a, sizeof(a)) < 0) {
        cout << "Connection failed\n";
        close(s);
        return 1;
    }

    char msg[1024];

    while (true) {
        cout << "Client: ";
        cin.getline(msg, sizeof(msg));

        send(s, msg, strlen(msg), 0);
        send(s, "\n", 1, 0);

        if (strcmp(msg, "exit") == 0) break;

        memset(msg, 0, sizeof(msg));
        int n = recv(s, msg, sizeof(msg) - 1, 0);

        if (n <= 0) break;

        cout << "Server: " << msg;
        if (strcmp(msg, "exit\n") == 0) break;
    }

    close(s);
    return 0;
}

// g++ Chat_TCP_Client.cpp -o filename
// ./Chat_TCP_Client
