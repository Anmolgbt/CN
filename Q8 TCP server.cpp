
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

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(s, (sockaddr*)&a, sizeof(a)) < 0 || listen(s, 5) < 0) {
        perror("Server");
        close(s);
        return 1;
    }

    cout << "Server started.\n";

    while (true) {
        cout << "Waiting for client..." << endl;
        int c = accept(s, nullptr, nullptr);

        if (c < 0) continue;

        double x, y, ans = 0;
        char op;

        bool valid =
            recv(c, &x, sizeof(x), MSG_WAITALL) == sizeof(x) &&
            recv(c, &y, sizeof(y), MSG_WAITALL) == sizeof(y) &&
            recv(c, &op, sizeof(op), MSG_WAITALL) == sizeof(op);

        if (valid) {
            cout << "Client connected." << endl;

            if (op == '+') ans = x + y;
            else if (op == '-') ans = x - y;
            else if (op == '*') ans = x * y;
            else if (op == '/' && y != 0) ans = x / y;
            else {
                cout << "Invalid operation or division by zero.\n";
                ans = 0;
            }

            send(c, &ans, sizeof(ans), 0);
            cout << "Server: Result = " << ans << endl;
        } else {
            cout << "Client disconnected without sending data." << endl;
            sleep(1);
        }

        close(c);
    }

    close(s);
    return 0;
}

// g++ filename.cpp -o filename
// ./filename
