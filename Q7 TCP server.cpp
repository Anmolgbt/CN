
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

    bind(s, (sockaddr*)&a, sizeof(a));
    listen(s, 1);
    cout << "Waiting for client...\n";

    int c = accept(s, nullptr, nullptr);
    char msg[1024] = {};
    recv(c, msg, sizeof(msg)-1, 0);
    cout << "Received: " << msg << '\n';

    close(c);
    close(s);
}


//to run 
//g++ server.cpp -o server
