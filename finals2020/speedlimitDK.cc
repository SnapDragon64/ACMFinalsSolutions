#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int64_t N, C;
vector<vector<pair<int64_t, int64_t>>> g;
vector<int64_t> alls;

pair<int64_t, vector<int64_t>> doit(int x, int prev) {
  int64_t mxs = 0, signcost = 0, ps = 0;
  for (auto [y, s] : g[x]) {
    mxs = max(mxs, s);
    if (y == prev) ps = s;
  };
  if (g[x].size() > 1) signcost += C * g[x].size();
  vector<int64_t> v;
  for (auto [y, s] : g[x]) if (y != prev) {
    auto [c2, v2] = doit(y, x);
    if (!v.size()) v.resize(alls.size());
    for (int i = 0; i < alls.size(); i++) v[i] += v2[i] + alls[i]-s;
    signcost += c2;
  };
  if (!v.size()) v.resize(alls.size());
  int64_t evencost = signcost;
  for (int i = 0; i < alls.size(); i++) {
    if (alls[i] >= mxs) {
      evencost = min(evencost, v[i] + (prev ? alls[i]-ps : 0));
      v[i] = min(v[i], signcost);
    } else {
      v[i] = signcost;
    }
  }
//cout << x << ": " << evencost << ' ';
//for (auto x : v) cout << ' ' << x;
//cout << endl;
  return {evencost, v};
}

int main() {
  int64_t U, V, S;
  while (cin >> N >> C) {
    g.clear();
    g.resize(N+1);
    for (int i = 1; i < N; i++) {
      cin >> U >> V >> S;
      g[U].push_back({V, S});
      g[V].push_back({U, S});
      alls.push_back(S);
    }
    sort(alls.begin(), alls.end());
    alls.erase(unique(alls.begin(), alls.end()), alls.end());
    auto [c, v] = doit(1, 0);
    cout << c << endl;
  }
}
