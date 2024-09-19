#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    string pos;
    cin >> pos;
    pos = string(N-pos.size(), '0') + pos;
    vector<int> d(N);
    for (int i = 0, t0 = 1, t1 = 3, tx = 2; i < N; i++) {
      d[i] = (pos[i] == '1' ? t1 : t0);
      swap(tx, pos[i] == '1' ? t0 : t1);
    }
    int big = -1, small = -1, next = -1;
    for (int i = 0  ; i <  N; i++) if (d[i] == 1) { big = i;   break; }
    for (int i = N-1; i >= 0; i--) if (d[i] == 3) { small = i; break; }
    for (int i = big-1; i >= 0; i--) if (d[i] == 2) { next = i; break; }

    auto normalize = [&](string b) {
      b = string(N+1-b.size(), 0) + b;
      for (int i = N, c = 0; i >= 0; i--) {
        b[i] += c;
        c = b[i]/2;
        b[i] %= 2;
      }
      return b;
    };
    auto solve = [&](const vector<int>& v, int big_bonus = 0) {
      string b(v.size(), 0);
      for (int i = 0, t = 3; i < v.size(); i++) if (v[i] != t) {
        b[i - (i < big ? big_bonus : 0)]++;
        t = (t == 1 || v[i] == 1) ? (t == 2 || v[i] == 2) ? 3 : 2 : 1;
      }
      return normalize(b);
    };

    string ret;
    bool first = true;
    if (big == -1 || small == -1 || big > small) {
      for (int i = 0; i < N; i++) d[i] = (d[i] == 1 ? 3 : d[i]);
      ret = solve(d);
    } else {
      for (int mvnext = 0; mvnext < 2; mvnext++) {
        vector<int> od = d;
        string mv;
        if (mvnext) {
          if (next == -1) continue;
          vector<int> mvd;
          for (int i = big; i < N; i++) if (d[i] != 3) { mvd.push_back(d[i] == 1 ? 3 : 2); d[i] = 1; }
          mv = solve(mvd);
          d[next] = 1;
          mv[N]++;
        }
        mv = normalize(mv);
        for (int t = 1; t <= 2; t++) {
          vector<int> fixd, lastd;
          for (int i = big+1; i < N; i++) if (d[i] != 3) fixd.push_back(d[i] + (d[i] == 2 && t == 1));
          for (int i = 0; i < N; i++) lastd.push_back(d[i] == 3 || i < big ? d[i] : 3-t);
          lastd[big] = t;
          string fix = solve(fixd), last = solve(lastd);
          fix[N]++;
          for (int i = 0; i <= N; i++) last[i] += fix[i] + mv[i];
          last = normalize(last);
          if (first || last < ret) ret = last;
          first = false;
        }
        d = od;
      }
    }

    int i;
    for (i = 0; i+1 < ret.size(); i++) if (ret[i]) break;
    for (i; i < ret.size(); i++) cout << int(ret[i]);
    cout << endl;
  }
}
