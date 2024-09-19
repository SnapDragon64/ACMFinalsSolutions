#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

map<vector<int64_t>, vector<vector<pair<int64_t, int64_t>>>> memo;
int64_t solve(int64_t m1, int64_t m2, int64_t m3, int64_t goal) {
  auto& v = memo[{m1, m2, m3}];
  int64_t m = max({m1, m2, m3});
  auto get = [&](int64_t g) {
    int64_t ret = 1e18;
    for (auto [x,n] : v[(g%m+m)%m]) ret = min(ret, n+abs(g-x)/m);
    return ret;
  };
  if (v.empty()) {
    v.resize(m);
    int64_t n = 0;
    vector<int64_t> q{0};
    for (int64_t n = 0; !q.empty(); n++) {
      vector<int64_t> q2;
      for (auto x : q) {
        if (n < get(x)) {
          v[(x%m+m)%m].push_back({x, n});
          if (m1 != -1) { q2.push_back(x+m1); q2.push_back(x-m1); }
          if (m2 != -1) { q2.push_back(x+m2); q2.push_back(x-m2); }
          if (m3 != -1) { q2.push_back(x+m3); q2.push_back(x-m3); }
        }
      }
      q.swap(q2);
    }
  }
  return get(goal);
}

int main() {
  int64_t A, B, C, X, Y;
  while (cin >> A >> B >> C >> X >> Y) {
    int64_t ret = 2e18;
    vector<int> xc(3000), yc(3000);
    function<void(int64_t,int64_t,int64_t,int64_t,int64_t,int64_t)> rec = [&](int64_t depth, int64_t a, int64_t b, int64_t c, int64_t x, int64_t y) {
      if (depth > 8) return;
      xc[a+c]++;
      yc[b+c]++;
      if (a == A && b == B && c == C) {
        ret = min(ret, depth + 2*solve(xc[a+b] ? a+b : -1, xc[a+c] ? a+c : -1, xc[b+c] ? b+c : -1, x)
                             + 2*solve(yc[a+b] ? a+b : -1, yc[a+c] ? a+c : -1, yc[b+c] ? b+c : -1, y));
      }
      rec(depth+1, c, b, a, x-a, y);
      rec(depth+1, c, b, a, x+c, y);
      rec(depth+1, a, c, b, x, y-b);
      rec(depth+1, a, c, b, x, y+c);
      yc[b+c]--;
      xc[a+c]--;
    };
    rec(0, A, B, C, X, Y);
    if (ret == 2e18) cout << "impossible" << endl; else cout << ret << endl;
  }
}
