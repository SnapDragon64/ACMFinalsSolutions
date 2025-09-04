#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template<typename T> constexpr T Gcd(const T& a, const T& b) { return b != 0 ? Gcd(b, a%b) : a < 0 ? -a : a; }

int main() {
  int64_t M;
  int N;
  while (cin >> M >> N) {
    vector<int> P(N);
    for (auto& x : P) cin >> x;
    sort(P.begin(), P.end());
    int g = 0;
    for (auto x : P) g = Gcd(g, x);

    vector<int> ret(9);
    ret[0] = 1;
    auto process = [&](int64_t s) {
      vector<int> cur(10);
      for (; s; s /= 10) cur[s%10]++;
      for (int i = 0; i < 9; i++) ret[i] = max(ret[i], cur[i]);
      ret[6] = max(ret[6], cur[6]+cur[9]);
    };
    process(M);

    priority_queue<int64_t> q;
    q.push(-0);
    int64_t last = -1e18, streak = 0;
    while (!q.empty()) {
      int64_t s = -q.top(); q.pop();
      if (s > M) s = M;
      if (s == last) continue;
      if (s == last+g) streak += g; else streak = 0;
      last = s;
      if (streak == P[0]) break;
      process(s);
      for (auto x : P) q.push(-(s+x));
    }

    for (int digit = 0; digit < 9; digit++) {
      static bool seen[19][2][2][2][1001][19];
      memset(seen, 0, sizeof(seen));
      function<void(int,int64_t,bool,bool,bool,int,int)> doit =
          [&](int curd, int64_t p10, bool zero, bool overLast, bool underM, int mod, int nd) {
        if (curd < 0) {
          if (mod == 0) ret[digit] = max(ret[digit], nd);
          return;
        }
        bool& s = seen[curd][zero][overLast][underM][mod][nd];
        if (s) return;
        s = true;
        int mind = 0, maxd = 9;
        if (!overLast) mind = (last/p10)%10;
        if (!underM)   maxd = (M   /p10)%10;
        for (int d = mind; d <= maxd; d++) {
          doit(curd-1, p10/10, zero && d==0, overLast || d>mind, underM || d<maxd, (mod*10+d)%g,
               nd + ((d == digit || (d == 9 && digit == 6)) && (d > 0 || !zero)));
        }
      };
      doit(18, 1e18, true, false, false, 0, 0);
    }

    for (int i = 0; i < 9; i++) if (ret[i]) cout << i << ' ' << ret[i] << endl;
  }
}
