
#include <iostream>
#include <cctype>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in a{}, client{};

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (sockaddr*)&a, sizeof(a));
    cout << "Server: Waiting for message...\n";

    char msg[1024] = {};
    socklen_t len = sizeof(client);
    int n = recvfrom(s, msg, sizeof(msg) - 1, 0,
                     (sockaddr*)&client, &len);

    if (n > 0) {
        for (int i = 0; i < n; i++)
            msg[i] = toupper((unsigned char)msg[i]);

        sendto(s, msg, n, 0, (sockaddr*)&client, len);
        cout << "Server: Uppercase = " << msg << '\n';
    }

    close(s);
    return 0;
}

// g++ filename.cpp -o filename
// ./filename
