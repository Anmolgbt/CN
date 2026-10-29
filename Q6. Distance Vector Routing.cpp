#include <iostream>
using namespace std;

#define INF 999
#define N 4

int main() {
    // Cost matrix: A, B, C, D
    int cost[N][N] = {
        {0,   2, INF, 1},
        {2,   0,   3, 7},
        {INF, 3,   0, 11},
        {1,   7,  11, 0}
    };

    int dist[N][N], nextHop[N][N];

    // Initialize distance and next hop
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = cost[i][j];

            if (cost[i][j] != INF && i != j)
                nextHop[i][j] = j;
            else
                nextHop[i][j] = -1;
        }
    }

    // Distance Vector Algorithm
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (cost[i][k] != INF &&
                    dist[k][j] != INF &&
                    dist[i][j] > cost[i][k] + dist[k][j]) {

                    dist[i][j] = cost[i][k] + dist[k][j];
                    nextHop[i][j] = k;
                }
            }
        }
    }

    // Print routing tables
    for (int i = 0; i < N; i++) {
        cout << "\nRouter " << char('A' + i) << "\n";
        cout << "Destination\tCost\tNext Hop\n";

        for (int j = 0; j < N; j++) {
            cout << char('A' + j) << "\t\t";

            if (dist[i][j] == INF)
                cout << "INF";
            else
                cout << dist[i][j];

            cout << "\t";

            if (i == j)
                cout << "-";
            else if (nextHop[i][j] == -1)
                cout << "-";
            else
                cout << char('A' + nextHop[i][j]);

            cout << "\n";
        }
    }

    // Find shortest path
    char s, d;
    cout << "\nEnter source and destination (A-D): ";
    cin >> s >> d;

    int source = s - 'A';
    int dest = d - 'A';

    if (source < 0 || source >= N || dest < 0 || dest >= N) {
        cout << "Invalid router!";
        return 0;
    }

    cout << "Shortest cost: " << dist[source][dest] << "\n";
    cout << "Path: " << s;

    int current = source;

    while (current != dest) {
        current = nextHop[current][dest];

        if (current == -1) {
            cout << "\nNo path exists";
            return 0;
        }

        cout << " -> " << char('A' + current);
    }

    cout << "\n";
    return 0;
}