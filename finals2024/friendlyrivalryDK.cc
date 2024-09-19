#include <algorithm>
#include <bitset>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<int64_t> X(2*N), Y(2*N);
    for (int i = 0; i < 2*N; i++) cin >> X[i] >> Y[i];

    vector<tuple<int64_t,int,int>> q;
    for (int i = 0; i < 2*N; i++)
    for (int j = 0; j < 2*N; j++) {
      q.push_back({(X[i]-X[j])*(X[i]-X[j]) + (Y[i]-Y[j])*(Y[i]-Y[j]), i, j});
    }
    sort(q.begin(), q.end());

    vector<int> team;
    vector<vector<int>> teams;
    for (int i = 0; i < 2*N; i++) { team.push_back(i); teams.push_back({i}); }
    auto merge = [&](int x, int y) {
      if (x > y) swap(x, y);
      bitset<501> b;  // More cheating with bitsets!
      b[0] = true;
      for (int k = 0; k < teams.size(); k++) {
        int sz = teams[k].size();
        if (k == x) sz += teams[y].size();
        if (k == y) sz = 0;
        if (sz) b |= (b<<sz);
      }
      if (!b[N]) return false;
      for (auto i : teams[y]) { team[i] = x; teams[x].push_back(i); }
      teams[y].clear();
      return true;
    };
    for (auto [d,i,j] : q) if (team[i] != team[j]) {
      if (!merge(team[i], team[j])) {
        cout << fixed << setprecision(10) << sqrt(d) << endl;
        break;
      }
    }
    for (int y = teams.size()-1; y >= 0; y--) if (teams[y].size())
    for (int x = y-1; x >= 0; x--) if (teams[x].size()) {
      if (merge(x, y)) break;
    }
    for (auto i : teams[0]) cout << i+1 << endl;
  }
}
