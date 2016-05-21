#include <cstring>
#include <iostream>
#include <set>
using namespace std;

#define MAXN 1000

int N;
int doll[MAXN+1], dollv[2014], retcost[MAXN+1];
int cost[MAXN+1][MAXN+1], mx[MAXN+1][MAXN+1];

main() {
  while (cin >> N) {
    for (int i = 0; i < N; i++) cin >> doll[i];
    memset(cost, 63, sizeof(cost));
    memset(mx, 0, sizeof(mx));
    for (int i = N-1; i >= 0; i--) {
      cost[i][i+1] = 0;
      memset(dollv, 0, sizeof(dollv));
      set<int> sorted;
      for (int j = i; j < N; j++) {
        if (dollv[doll[j]]) break;
        dollv[doll[j]] = true;
        mx[i][j+1] = max(mx[i][j], doll[j]);
        sorted.insert(doll[j]);

        set<int>::iterator smallest = sorted.end();
        --smallest;
        int seqsize1 = sorted.size(), seqsize2 = 0;
        for (int k = j+1; k < N; k++) {
          if (dollv[doll[k]]) break;
          while (seqsize1 && doll[k] < *smallest) {
            --seqsize1;
            if (smallest != sorted.begin()) --smallest;
          }
          if (!seqsize1 && doll[k] < *smallest) {
            ++seqsize2;
          }
          cost[i][k+1] = min(cost[i][k+1], cost[i][j+1] + cost[j+1][k+1] +
              (k+1-i) - max(seqsize1, seqsize2));
        }
      }
    }

    memset(retcost, 63, sizeof(retcost));
    retcost[0] = 0;
    for (int i = 0; i < N; i++) if (retcost[i] < 1000000000) {
      for (int j = i; j < N; j++) if (mx[i][j+1] == j+1-i) {
        retcost[j+1] = min(retcost[j+1], retcost[i] + cost[i][j+1]);
      }
    }
    if (retcost[N] < 1000000000) {
      cout << retcost[N] << endl;
    } else {
      cout << "impossible" << endl;
    }
  }
}
