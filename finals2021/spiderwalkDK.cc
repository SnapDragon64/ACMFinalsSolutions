#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
  int N, M, S, D, T;
  while (cin >> N >> M >> S) {
    S--;
    vector<pair<int, int>> b;
    for (int i = 0; i < M; i++) {
      cin >> D >> T;
      b.push_back({-D, T-1});
    }
    sort(b.begin(), b.end());

    map<int, int> m;
    m[S] = 1;
    m[(S+N/2)%N] = 0;
    m[(S+(N+1)/2)%N] = -1;
    auto pred = [&](map<int,int>::iterator it) { return --(it == m.begin() ? m.end() : it); };
    auto succ = [&](map<int,int>::iterator it) { ++it; return (it == m.end() ? m.begin() : it); };
    auto getAll = [&](int x) {
      auto it = pred(m.upper_bound(x)), pit = it, nit = succ(it);
      if (pit->first == x) pit = pred(pit);
      if (nit->first != (x+1)%N) nit = it;
      return make_tuple(pit, it, nit);
    };
    auto set = [&](int x, int d) {
      auto [pit, it, nit] = getAll(x);
      int xd = it->second, pd = pit->second, nd = nit->second;
      if (xd == d) return;
      if (d == pd) m.erase(x); else m[x] = d;
      if (nd == d) m.erase((x+1)%N); else m[(x+1)%N] = nd;
    };
    auto swp = [&](int x) {
      auto [pit, it, nit] = getAll(x);
      int xd = it->second, pd = pit->second, nd = nit->second;
      if (xd == 0) return;
      xd = -xd; pd -= xd; nd -= xd;
      if (pd ==  2) { pd--; xd++; }
      if (nd == -2) { nd++; xd--; }
      if (pd == -2) {
        pd++;
        set((pit->first + (N-1))%N, pred(pit)->second - 1);
      }
      if (nd == 2) {
        nd--;
        set(succ(nit)->first, succ(nit)->second + 1);
      }
      set((x+N-1)%N, pd);
      set(x, xd);
      set((x+1)%N, nd);
    };

    int s = S;
    for (auto [_, t] : b) {
      swp(t);
      if (s == t) s = (s+1)%N; else if (s == (t+1)%N) s = t;
    }

    vector<int> ret(N);
    for (int i = 0, cur = 0, d = 0; i < N; i++) {
      ret[s] = cur;
      if (m.count(s)) d = m[s];
      cur += d;
      s = (s+1)%N;
    }
    for (auto x : ret) cout << x << endl;
  }
}
