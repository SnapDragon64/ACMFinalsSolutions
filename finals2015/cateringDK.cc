#include <iostream>
#include <vector>
using namespace std;

main() {
  int N, K;
  while (cin >> N >> K) {
    int nv = 2*N+3;
    vector<vector<int> > cap(nv, vector<int>(nv));
    vector<vector<int> > cost(nv, vector<int>(nv));
    for (int i = 0; i <= N; i++) {
      cap[N+1+i][i] = 1;
      cost[N+1+i][i] = -10000000;
      cost[i][N+1+i] = 10000000;
      cap[i][nv-1] = N;
      for (int j = i+1; j <= N; j++) {
        cap[i][N+1+j] = 1;
        cin >> cost[i][N+1+j];
        cost[N+1+j][i] = -cost[i][N+1+j];
      }
    }
    int ret = 10000000 * N;
    for (int f = 0; f < min(N, K); f++) {
      vector<int> best(nv, 1000000000), prev(nv, -1);
      best[0] = 0;
      vector<bool> changed(nv, true);
      for (int i = 0; i < nv; i++) {
        vector<bool> changed2(nv, false);
        for (int j = 0; j < nv; j++) if (changed[j])
        for (int k = 0; k < nv; k++) if (cap[j][k]) {
          if (best[k] > best[j] + cost[j][k]) {
            best[k] = best[j] + cost[j][k];
            prev[k] = j;
            changed2[k] = true;
          }
        }
        changed = changed2;
      }
      ret += best[nv-1];
      for (int x = nv-1; x != 0; x = prev[x]) {
        cap[prev[x]][x]--;
        cap[x][prev[x]]++;
      }
    }
    cout << ret << endl;
  }
}
