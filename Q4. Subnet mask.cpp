
#include <bits/stdc++.h>
using namespace std;

int main() {
    int prefix, subnets;

    // Input 1: Original network prefix (e.g., 24 for /24)
    // Input 2: Number of subnets required (e.g., 4)
    // Example input: 24 4
    cout << "Enter original prefix and required subnets: ";
    cin >> prefix >> subnets;

    int bits = 0;
    while ((1 << bits) < subnets)
        bits++;

    int newPrefix = prefix + bits;

    int hostBits = 32 - newPrefix;

    if (prefix < 0 || prefix > 30 ||
        subnets < 1 || newPrefix > 30) {
        cout << "Invalid input\n";
        return 0;
    }

    // Calculate subnet mask
    unsigned int mask = 0xFFFFFFFFu << hostBits;

    cout << "New prefix: /" << newPrefix << '\n';

    
    cout << "Subnet mask: "
         << ((mask >> 24) & 255) << "."
         << ((mask >> 16) & 255) << "."
         << ((mask >> 8) & 255) << "."
         << (mask & 255) << '\n';

    // Total addresses = 2^hostBits
    // Usable hosts = total addresses - 2
    cout << "Usable hosts per subnet: "
         << ((1u << hostBits) - 2) << '\n';

    return 0;
}
