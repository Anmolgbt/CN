
#include <bits/stdc++.h>
using namespace std;

int main() {
    int b[8] = {0}; // positions 1 to 7
    cout << "Enter 4 data bits: ";

    cin >> b[3] >> b[5] >> b[6] >> b[7];

    b[1] = b[3] ^ b[5] ^ b[7];
    b[2] = b[3] ^ b[6] ^ b[7];
    b[4] = b[5] ^ b[6] ^ b[7];

    cout << "Hamming code: ";
    for (int i = 1; i <= 7; i++) cout << b[i];
    cout << "\nEnter error position (0-7): ";

    int e;
    cin >> e;
    if (e >= 1 && e <= 7) b[e] ^= 1;

    int s1 = b[1] ^ b[3] ^ b[5] ^ b[7];
    int s2 = b[2] ^ b[3] ^ b[6] ^ b[7];
    int s4 = b[4] ^ b[5] ^ b[6] ^ b[7];

    int error = s1 + 2*s2 + 4*s4;
    cout << "Error position: " << error << '\n';

    if (error) b[error] ^= 1;

    cout << "Corrected code: ";
    for (int i = 1; i <= 7; i++) cout << b[i];
    cout << '\n';
}
