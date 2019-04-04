#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int P, H;
  while (cin >> P >> H) {
    vector<vector<int>> S(P, vector<int>(H));
    vector<int64_t> tot(P);
    for (int i = 0; i < P; i++) {
      for (int j = 0; j < H; j++) {
        cin >> S[i][j];
        tot[i] += S[i][j];
      }
      S[i].push_back(0);
      sort(S[i].begin(), S[i].end(), greater<int>());
    }

    for (int i = 0; i < P; i++) {
      vector<pair<int, int>> events;
      int cur = 0;
      for (int j = 0; j < P; j++) {
        int64_t itot = tot[i], jtot = tot[j], lim = 1000000000;
        if (jtot <= itot) cur++;
        for (int ih = 0, jh = 0; ih < H || jh < H; S[i][ih] > S[j][jh] ? ih++ : jh++) {
          bool old = (jtot <= itot);
          int v = max(S[i][ih], S[j][jh]);
          itot -= (lim-v) * ih; jtot -= (lim-v) * jh;
          lim = v;
          if (!old && jtot <= itot) {
            events.emplace_back(lim+(itot-jtot)/(jh-ih), 1);
          } else if (old && jtot > itot) {
            events.emplace_back(lim+(jtot-itot-1)/(ih-jh), -1);
          }
        }
      }

      sort(events.begin(), events.end(), greater<pair<int, int>>());
      int ret = cur;
      for (auto const& e : events) { cur += e.second; ret = min(ret, cur); }
      cout << ret << endl;
    }
  }
}
