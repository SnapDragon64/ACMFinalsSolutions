#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<vector<int>> v(1000001);
    vector<int> cum(N+1), tot(N+1);
    for (int i = 1; i <= N; i++) {
      char ch;
      int s;
      cin >> ch >> s;
      v[s].push_back((ch == 's') ? i : ~i);
    }
    for (int i = 0; i < v.size(); i++) if (v[i].size()) {
      int cur = 0, mn = 0;
      for (int j = 0; j < v[i].size(); j++) {
        if (v[i][j] >= 0) cur++; else cur--;
        mn = min(mn, cur);
      }
      if (cur != 0) continue;
      for (int j = 0; j < v[i].size(); j++) {
        if (v[i][j] >= 0) cur++; else cur--;
        if (mn == cur) {
          cum[~v[i][j]]++;
          if (j+1 == v[i].size()) {
            cum[0]++;
            cum[v[i][0]]--;
          } else {
            cum[v[i][j+1]]--;
          }
        }
      }
    }
    int cur = 0, mx = 0, mxi = 1;
    for (int i = 0; i < cum.size(); i++) {
      tot[i] += cur;
      if (tot[i] > mx) { mx = tot[i]; mxi = i; }
      cur += cum[i];
    }
    cout << mxi << ' ' << mx << endl;
  }
}
