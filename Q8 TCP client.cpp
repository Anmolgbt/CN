
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

    if (connect(s, (sockaddr*)&a, sizeof(a)) < 0) {
        perror("Connection failed");
        close(s);
        return 1;
    }

    double x, y, ans;
    char op;

    cout << "Enter expression (e.g. 10 + 5): ";
    cin >> x >> op >> y;

    if (send(s, &x, sizeof(x), 0) != sizeof(x) ||
        send(s, &y, sizeof(y), 0) != sizeof(y) ||
        send(s, &op, sizeof(op), 0) != sizeof(op)) {
        cout << "Sending failed.\n";
        close(s);
        return 1;
    }

    if (recv(s, &ans, sizeof(ans), MSG_WAITALL) == sizeof(ans))
        cout << "Client: Answer = " << ans << endl;
    else
        cout << "No result received.\n";

    close(s);
    return 0;
}

// g++ filename.cpp -o filename
// ./filename
