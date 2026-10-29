
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    connect(s, (sockaddr*)&a, sizeof(a));

    double x, y, ans;
    char op;
    cout << "Enter number operator number: ";
    cin >> x >> op >> y;

    send(s, &x, sizeof(x), 0);
    send(s, &y, sizeof(y), 0);
    send(s, &op, sizeof(op), 0);
    recv(s, &ans, sizeof(ans), 0);

    cout << "Answer: " << ans << '\n';
    close(s);
}

// to run
// g++ client.cpp -o client