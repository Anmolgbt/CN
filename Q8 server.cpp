
#include <iostream>
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
    int c = accept(s, nullptr, nullptr);

    double x, y, ans = 0;
    char op;
    recv(c, &x, sizeof(x), 0);
    recv(c, &y, sizeof(y), 0);
    recv(c, &op, sizeof(op), 0);

    if (op == '+') ans = x + y;
    else if (op == '-') ans = x - y;
    else if (op == '*') ans = x * y;
    else if (op == '/' && y != 0) ans = x / y;
    else {
        ans = 0;
        cout << "Invalid operation\n";
    }

    send(c, &ans, sizeof(ans), 0);
    cout << "Result: " << ans << '\n';
    close(c);
    close(s);
}

//to run 
//g++ server.cpp -o server
