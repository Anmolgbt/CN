
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in a{};

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);

    string msg;
    cout << "Client: Enter string: ";
    getline(cin, msg);

    sendto(s, msg.c_str(), msg.size(), 0,
           (sockaddr*)&a, sizeof(a));

    char reply[1024] = {};
    int n = recvfrom(s, reply, sizeof(reply) - 1, 0,
                     nullptr, nullptr);

    if (n > 0)
        cout << "Client: Uppercase = " << reply << '\n';

    close(s);
    return 0;
}

// g++ filename.cpp -o filename
// ./filename
