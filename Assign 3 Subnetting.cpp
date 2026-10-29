
#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
using namespace std;

// Convert IPv4 address into 4 octets
bool validIP(string ip, int a[]) {
    stringstream ss(ip);
    string part;
    int i = 0;

    while (getline(ss, part, '.')) {
        if (i >= 4 || part.empty())
            return false;

        for (char c : part)
            if (c < '0' || c > '9')
                return false;

        try {
            size_t pos;
            int x = stoi(part, &pos);
            if (pos != part.size() || x < 0 || x > 255)
                return false;
            a[i++] = x;
        } catch (...) {
            return false;
        }
    }
    return i == 4;
}

// Convert IP address to integer
uint32_t toInt(int a[]) {
    return ((uint32_t)a[0] << 24) |
           ((uint32_t)a[1] << 16) |
           ((uint32_t)a[2] << 8) | a[3];
}

// Convert integer to IP address
string toIP(uint32_t x) {
    return to_string((x >> 24) & 255) + "." +
           to_string((x >> 16) & 255) + "." +
           to_string((x >> 8) & 255) + "." +
           to_string(x & 255);
}

// Display classful IP information
void classInfo(int a[]) {
    int first = a[0];
    char cls;
    string mask, range;
    int netBits = 0, hostBits = 0;
    unsigned long long networks = 0, hosts = 0;

    cout << "\n--- CLASSFUL IP ANALYSIS ---\n";
    cout << "First Octet: " << first << "\n";
    cout << "IP Address Binary: ";

    for (int i = 7; i >= 0; i--)
        cout << ((first >> i) & 1);
    cout << "\n";

    if (first == 127) {
        cout << "IP Class: Loopback\n";
        return;
    } else if (first >= 1 && first <= 126) {
        cls = 'A';
        range = "1.0.0.0 - 126.255.255.255";
        mask = "255.0.0.0";
        netBits = 8;
        hostBits = 24;
        networks = 126;
        hosts = (1ULL << 24) - 2;
    } else if (first >= 128 && first <= 191) {
        cls = 'B';
        range = "128.0.0.0 - 191.255.255.255";
        mask = "255.255.0.0";
        netBits = 16;
        hostBits = 16;
        networks = 16384;
        hosts = (1ULL << 16) - 2;
    } else if (first >= 192 && first <= 223) {
        cls = 'C';
        range = "192.0.0.0 - 223.255.255.255";
        mask = "255.255.255.0";
        netBits = 24;
        hostBits = 8;
        networks = 2097152;
        hosts = 254;
    } else if (first >= 224 && first <= 239) {
        cls = 'D';
        range = "224.0.0.0 - 239.255.255.255";
        mask = "N/A";
    } else {
        cls = 'E';
        range = "240.0.0.0 - 255.255.255.255";
        mask = "N/A";
    }

    cout << "IP Class: " << cls << "\n";
    cout << "IP Range: " << range << "\n";
    cout << "Default Subnet Mask: " << mask << "\n";

    if (cls >= 'A' && cls <= 'C') {
        cout << "Network Bits: " << netBits << "\n";
        cout << "Host Bits: " << hostBits << "\n";
        cout << "Usable Hosts: " << hosts << "\n";
        cout << "Number of Networks: " << networks << "\n";
        cout << "Host Formula: 2^" << hostBits << " - 2\n";
    } else if (cls == 'D') {
        cout << "Purpose: Multicast\n";
    } else {
        cout << "Purpose: Experimental / Reserved\n";
    }

    cout << "\n--- SPECIAL ADDRESS CHECK ---\n";
    bool special = false;
    uint32_t value = toInt(a);

    if (value == 0) {
        cout << "Default / Unspecified Address\n";
        special = true;
    }
    if (first == 127) {
        cout << "Loopback Address\n";
        special = true;
    }
    if (value == 0xFFFFFFFFu) {
        cout << "Limited Broadcast Address\n";
        special = true;
    }
    if (first == 169 && a[1] == 254) {
        cout << "APIPA / Link-Local Address\n";
        special = true;
    }
    if (first == 10) {
        cout << "Private Class A Address\n";
        special = true;
    }
    if (first == 172 && a[1] >= 16 && a[1] <= 31) {
        cout << "Private Class B Address\n";
        special = true;
    }
    if (first == 192 && a[1] == 168) {
        cout << "Private Class C Address\n";
        special = true;
    }
    if (first >= 224 && first <= 239) {
        cout << "Multicast Address\n";
        special = true;
    }
    if (first >= 240) {
        cout << "Experimental / Reserved Address\n";
        special = true;
    }
    if (!special)
        cout << "Normal Unicast IPv4 Address\n";
}

// Perform subnetting
void subnetting(int a[], int required) {
    int first = a[0];
    int netBits;

    if (first >= 1 && first <= 126)
        netBits = 8;
    else if (first >= 128 && first <= 191)
        netBits = 16;
    else if (first >= 192 && first <= 223)
        netBits = 24;
    else {
        cout << "\nSubnetting is not supported for this IP class.\n";
        return;
    }

    int borrowed = 0;
    unsigned long long count = 1;

    while (count < (unsigned long long)required) {
        borrowed++;
        count *= 2;
    }

    int prefix = netBits + borrowed;

    if (prefix > 30) {
        cout << "\nToo many subnets requested.\n";
        cout << "At least 2 host bits are required.\n";
        return;
    }

    int hostBits = 32 - prefix;
    uint32_t addresses = 1u << hostBits;
    uint32_t usable = addresses - 2;

    uint32_t mask = 0xFFFFFFFFu << hostBits;
    uint32_t network = toInt(a) & mask;

    cout << "\n--- SUBNETTING DETAILS ---\n";
    cout << "Required Subnets: " << required << "\n";
    cout << "Borrowed Bits: " << borrowed << "\n";
    cout << "Actual Number of Subnets: " << count << "\n";
    cout << "Subnet Prefix: /" << prefix << "\n";
    cout << "Host Bits: " << hostBits << "\n";
    cout << "Addresses per Subnet: " << addresses << "\n";
    cout << "Usable IPs per Subnet: " << usable << "\n";
    cout << "Subnet Mask: " << toIP(mask) << "\n";

    cout << "\n--- SUBNET DETAILS ---\n";

    for (uint64_t i = 0; i < count; i++) {
        uint32_t net = network + (uint32_t)i * addresses;
        uint32_t broadcast = net + addresses - 1;

        cout << "\nSubnet " << i + 1 << "\n";
        cout << "Network Address: " << toIP(net) << "\n";
        cout << "First IP: " << toIP(net + 1) << "\n";
        cout << "Last IP: " << toIP(broadcast - 1) << "\n";
        cout << "Broadcast Address: " << toIP(broadcast) << "\n";
    }
}

int main() {
    string ip;
    int a[4];
    int required;

    cout << "IPv4 CLASSFUL ADDRESSING AND SUBNETTING\n";

    do {
        cout << "\nEnter IPv4 address: ";
        cin >> ip;

        if (!validIP(ip, a))
            cout << "Invalid IP! Example: 195.1.1.10\n";
    } while (!validIP(ip, a));

    classInfo(a);

    cout << "\nEnter number of required subnets: ";
    cin >> required;

    if (cin.fail() || required <= 0) {
        cout << "Invalid number of subnets.\n";
        return 0;
    }

    subnetting(a, required);

    cout << "\nProgram completed.\n";
    return 0;
}
