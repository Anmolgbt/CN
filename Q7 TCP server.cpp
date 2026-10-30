
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
    a.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(s, (sockaddr*)&a, sizeof(a)) < 0 ||
        listen(s, 5) < 0) {
        perror("Server");
        close(s);
        return 1;
    }

    char msg[1024];

    while (true) {
        cout << "Server: Waiting for client..." << endl;
        int c = accept(s, nullptr, nullptr);

        if (c < 0) continue;

        cout << "Server: Client connected." << endl;

        while (true) {
            memset(msg, 0, sizeof(msg));
            int n = recv(c, msg, sizeof(msg) - 1, 0);

            if (n <= 0) {
                cout << "Server: Client disconnected." << endl;
                break;
            }

            msg[n] = '\0';
            cout << "Client: " << msg;

            if (strncmp(msg, "exit", 4) == 0) break;

            cout << "Server: ";
            cin.getline(msg, sizeof(msg));

            send(c, msg, strlen(msg), 0);
            send(c, "\n", 1, 0);

            if (strcmp(msg, "exit") == 0) break;
        }

        close(c);
    }

    close(s);
    return 0;
}

// g++ Chat_TCP_Server.cpp -o filename
// ./Chat_TCP_Server
