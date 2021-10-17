#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

static vector<vector<vector<int>>> partitions{
  {},
  {{1}},
  {{2}, {1,1}},
  {{3}, {2,1}, {1,2}, {1,1,1}},
  {{4}, {3,1}, {2,2}, {2,1,1}, {1,3}, {1,2,1}, {1,1,2}, {1,1,1,1}}
};

int N;
vector<vector<pair<int, int>>> g;

vector<int> doit(int x, int prev) {
  vector<vector<pair<int, int>>> best(5);
  for (auto [y, d] : g[x]) if (y != prev) {
    auto v = doit(y, x);
    for (int i = 1; i <= 4; i++) best[i].push_back({v[i] + (i%2 ? d : 0), y});
  }
  for (int i = 1; i <= 4; i++) {
    sort(best[i].begin(), best[i].end(), greater<pair<int, int>>());
  }

  vector<int> ret(5);
  for (int i = 1; i <= 4; i++) {
    for (auto const& partition : partitions[i]) {
      vector<int> seen, idx(5);
      int cur = 0;
      for (auto x : partition) {
        for (; idx[x] < best[x].size(); idx[x]++) {
          auto [value, nd] = best[x][idx[x]];
          if (find(seen.begin(), seen.end(), nd) == seen.end()) {
            seen.push_back(nd);
            cur += value;
            break;
          }
        }
      }
      ret[i] = max(ret[i], cur);
    }
  }
  return ret;
}

int main() {
  while (cin >> N) {
    int ret = 0;
    g.clear();
    g.resize(N+1);
    for (int i = 0; i < N-1; i++) {
      int A, B, D;
      cin >> A >> B >> D;
      g[A].push_back({B, D});
      g[B].push_back({A, D});
      ret += D*2;
    }
    cout << ret - doit(rand()%N+1, 0)[4] << endl;
  }
}
