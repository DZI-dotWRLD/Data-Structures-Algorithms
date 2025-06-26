#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

// Number of Queens or Kings
#define N 4


bool prefersNewQueenOverCurrent(int kingPrefs[N][N], int king, int newQueen, int currentQueen) {
    for (int i = 0; i < N; i++) {
        if (kingPrefs[king][i] == newQueen) return true;  
        if (kingPrefs[king][i] == currentQueen) return false; 
    }
    return false;
}

// Gale-Shapley 
void galeShapleyStableMarriage(vector<string> queens, vector<string> kings,
    int queenPrefs[N][N], int kingPrefs[N][N]) {
    int engagements[N];       
    bool queenFree[N];        
    memset(engagements, -1, sizeof(engagements));
    memset(queenFree, true, sizeof(queenFree));

    int freeCount = N; // Number of free queens
    int proposalCount[N] = { 0 }; // Tracks number of proposals 

    cout << "Initial Proposals and Engagements\n------------------------------\n";

    int round = 1;
    while (freeCount > 0) {
        cout << "Round " << round << endl;

        
        vector<string> proposals;

        for (int q = 0; q < N; q++) {
            if (queenFree[q]) {
                int king = queenPrefs[q][proposalCount[q]]; // Next king in queen's preference list
                proposalCount[q]++; 

                // Check if the king is free
                if (engagements[king] == -1) {
                    engagements[king] = q; // Engage the king with this queen
                    queenFree[q] = false;
                    freeCount--;
                }
                else {
                    // The king is already engaged, compare preferences
                    int currentQueen = engagements[king];
                    if (prefersNewQueenOverCurrent(kingPrefs, king, q, currentQueen)) {
                        // King prefers the new queen, break engagement with current queen
                        queenFree[currentQueen] = true;
                        engagements[king] = q; // Engage with new queen
                        queenFree[q] = false;
                    }
                    else {
                        // King rejects the new queen, she remains free
                        proposals.push_back(queens[q]);
                    }
                }
            }
        }

        
        cout << "Queens proposal to:\n";
        for (int i = 0; i < N; i++) {
            if (queenFree[i]) cout << "    " << queens[i] << endl;
        }

        cout << "\nKings engaged to:\n";
        for (int k = 0; k < N; k++) {
            if (engagements[k] != -1) {
                cout << "    " << kings[k] << " -> " << queens[engagements[k]] << endl;
            }
        }

        cout << "------------------------------\n";
        round++;
    }

    
    cout << "\nFinal Engagements:\n";
    for (int k = 0; k < N; k++) {
        cout << "    " << kings[k] << " -> " << queens[engagements[k]] << endl;
    }
}

int main() {
    
    vector<string> queens = { "QS", "QH", "QD", "QC" };
    vector<string> kings = { "KH", "KS", "KC", "KD" };

    // Preferences of Queens (Queens proposing to Kings)
    int queenPrefs[N][N] = {
        {1, 3, 0, 2},  // QS: KS, KD, KH, KC
        {2, 3, 1, 0},  // QH: KC, KD, KS, KH
        {1, 2, 3, 0},  // QD: KS, KC, KD, KH
        {1, 2, 3, 0}   // QC: KS, KC, KD, KH
    };

    // Preferences of Kings
    int kingPrefs[N][N] = {
        {2, 1, 3, 0},  // KH: QD, QH, QC, QS
        {2, 1, 3, 0},  // KS: QD, QH, QC, QS
        {0, 2, 1, 3},  // KC: QC, QD, QH, QS
        {1, 2, 0, 3}   // KD: QH, QD, QS, QC
    };

    galeShapleyStableMarriage(queens, kings, queenPrefs, kingPrefs);

    return 0;
}
