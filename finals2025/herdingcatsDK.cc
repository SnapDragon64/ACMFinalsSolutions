#include <iostream>
#include <vector>
using namespace std;

int main() {
  int T, N, M, K;
  for (cin >> T; T--;) {
    cin >> N >> M;
    vector<int> P(N+1), catn(M+1), plantn(M+1), plantpos(M+1), plantposn(M+1);
    vector<vector<int>> like(N+1);
    for (int cat = 1; cat <= N; cat++) {
      cin >> P[cat] >> K;
      catn[P[cat]]++;
      like[cat].resize(K);
      for (auto& plant : like[cat]) {
        cin >> plant;
        if (P[cat] > plantpos[plant]) {
          plantpos[plant] = P[cat];
          plantposn[plant] = 1;
        } else if (P[cat] == plantpos[plant]) {
          plantposn[plant]++;
        }
      }
    }

    bool fail = false;
    for (int plant = 1; plant <= M; plant++) plantn[plantpos[plant]]++;
    for (int pos = M, tot = 0; pos >= 1; pos--) {
      tot += plantn[pos];
      if (tot > M-pos+1) fail = true;
    }
    for (int cat = 1; cat <= N; cat++) {
      bool found = false;
      for (auto plant : like[cat]) if (plantpos[plant] == P[cat] && plantposn[plant] == catn[P[cat]]) found = true;
      fail |= !found;
    }
    cout << (fail ? "NO" : "YES") << endl;
  }
}
