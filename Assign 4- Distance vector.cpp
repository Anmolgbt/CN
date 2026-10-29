
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

const int INF = 999;

struct Route {
    int cost, nextHop;
};

char name(int r) {
    return 'A' + r;
}

void printTables(vector<vector<Route>>& rt) {
    int n = rt.size();

    for (int i = 0; i < n; i++) {
        cout << "\nRouting Table of Router " << name(i) << "\n";
        cout << left << setw(15) << "Destination"
             << setw(10) << "Cost" << "Next Hop\n";

        for (int j = 0; j < n; j++) {
            cout << left << setw(15) << name(j);

            if (rt[i][j].cost >= INF)
                cout << setw(10) << "INF";
            else
                cout << setw(10) << rt[i][j].cost;

            if (rt[i][j].nextHop == -1)
                cout << "-";
            else
                cout << name(rt[i][j].nextHop);

            cout << "\n";
        }
    }
}

void shortestPath(vector<vector<Route>>& rt, int s, int d) {
    int n = rt.size();

    cout << "\nShortest Distance: ";

    if (rt[s][d].cost >= INF) {
        cout << "INF\nNo path exists.\n";
        return;
    }

    cout << rt[s][d].cost << "\nShortest Path: ";

    int current = s;
    cout << name(current);

    for (int i = 0; current != d && i < n; i++) {
        current = rt[current][d].nextHop;

        if (current == -1) {
            cout << "\nNo path exists.\n";
            return;
        }

        cout << " -> " << name(current);
    }

    if (current != d)
        cout << "\nUnable to construct path.\n";
    else
        cout << "\n";
}

int main() {
    const int n = 4;

    // Cost matrix: A, B, C, D
    vector<vector<int>> cost = {
        {0,   2, INF,  1},
        {2,   0,   3,  7},
        {INF, 3,   0, 11},
        {1,   7,  11,  0}
    };

    vector<vector<Route>> rt(n, vector<Route>(n));

    // Initialize routing tables
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            rt[i][j].cost = cost[i][j];
            rt[i][j].nextHop =
                (i == j) ? i : (cost[i][j] >= INF ? -1 : j);
        }
    }

    cout << "DISTANCE VECTOR ROUTING\n";
    cout << "\nInitial Routing Tables:\n";
    printTables(rt);

    int iteration = 0;

    // Distance Vector Algorithm
    while (true) {
        iteration++;
        bool updated = false;

        vector<vector<Route>> newTable = rt;

        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                if (i == k || cost[i][k] >= INF)
                    continue;

                for (int j = 0; j < n; j++) {
                    if (rt[k][j].cost >= INF)
                        continue;

                    int newCost = cost[i][k] + rt[k][j].cost;

                    if (newCost < newTable[i][j].cost) {
                        newTable[i][j].cost = newCost;
                        newTable[i][j].nextHop = k;
                        updated = true;
                    }
                }
            }
        }

        rt = newTable;

        cout << "\nIteration " << iteration << ":\n";
        printTables(rt);

        if (!updated)
            break;
    }

    cout << "\nFinal Routing Tables:\n";
    printTables(rt);

    cout << "\nConverged after " << iteration
         << " iterations.\n";

    char source, destination;

    cout << "\nEnter Source Router (A-D): ";
    cin >> source;

    cout << "Enter Destination Router (A-D): ";
    cin >> destination;

    // Convert to uppercase
    if (source >= 'a' && source <= 'd')
        source -= 32;
    if (destination >= 'a' && destination <= 'd')
        destination -= 32;

    if (source < 'A' || source > 'D' ||
        destination < 'A' || destination > 'D') {
        cout << "Invalid router entered.\n";
        return 0;
    }

    int s = source - 'A';
    int d = destination - 'A';

    cout << "\nSource: " << source;
    cout << "\nDestination: " << destination << "\n";

    shortestPath(rt, s, d);

    return 0;
}
