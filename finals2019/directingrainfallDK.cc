#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

struct Tarp {
  int idx, X1, Y1, X2, Y2;
  bool operator<(const Tarp& t) const {
    if (X1 > t.X1) {
      return int64_t(Y1-t.Y1)*(t.X2-t.X1) < int64_t(X1-t.X1)*(t.Y2-t.Y1);
    } else {
      return int64_t(t.Y1-Y1)*(X2-X1) > int64_t(t.X1-X1)*(Y2-Y1);
    }
  }
};

struct Deltas {
  int base;
  map<int, int> d;
  map<int, int>::iterator mid;
  Deltas(int l, int r) : base(0) {
    d[-2e9] = d[l] = d[r] = d[2e9] = 1e8;
    mid = d.find(l);
  }

  int MidL() { return mid->first; }
  int MidR() { auto it = mid; return (++it)->first; }
  void Add(int x, int v, bool leftward) {
    if (v == 0) return;
    d[x] += v;
    if (leftward && x > MidL()) ++mid;
  }
  bool SubNearest(int x1, int x2, bool left) {
    auto it = left ? d.lower_bound(x1) : --d.upper_bound(x2);
    if (it->first < x1 || it->first > x2) return false;
    if (--it->second == 0) { if (it == mid) --mid; d.erase(it); }
    return true;
  }
  int Collect(int x1, int x2) {
    int tot = 0;
    for (auto it = d.lower_bound(x1); it->first <= x2; ) {
      tot += it->second;
      if (it == mid) --mid;
      d.erase(it++);
    }
    return tot;
  }
};

int main() {
  int L, R, N;
  while (cin >> L >> R >> N) {
    vector<Tarp> T(N);
    for (int i = 0; i < N; i++) {
      cin >> T[i].X1 >> T[i].Y1 >> T[i].X2 >> T[i].Y2;
      if (T[i].X1 > T[i].X2) { swap(T[i].X1, T[i].X2); swap(T[i].Y1, T[i].Y2); }
    }
    sort(T.begin(), T.end(), [] (const Tarp& a, const Tarp& b) { return a.X1 < b.X1; });
    for (int i = 0; i < N; i++) T[i].idx = i;

    vector<vector<int>> succ(N+1);
    set<Tarp> s;
    priority_queue<pair<int, int>> events;
    for (int i = 0; i < N; i++) {
      while (events.size() && -events.top().first < T[i].X1) {
        s.erase(T[events.top().second]);
        events.pop();
      }
      auto it = s.insert(T[i]).first;
      if (it == s.begin()) succ[N].push_back(i); else succ[(--it)->idx].push_back(i);
      events.push({-T[i].X2, i});
    }

    Deltas d(L, R);
    function<void(int)> doit = [&] (int n) {
      for (int i = succ[n].size()-1; i >= 0; i--) {
        const Tarp& t = T[succ[n][i]];
        if (t.Y1 < t.Y2) {
          if (t.X2 < d.MidR()) {
            if (d.SubNearest(t.X1, t.X2, true)) d.Add(t.X2, 1, true);
          } else if (t.X1 < d.MidL()) {
            d.SubNearest(t.X1, t.X2, true);
            d.base++;
            d.Add(t.X2, d.Collect(d.MidR(), t.X2)-1, false);
          } else {
            d.Add(t.X2, d.Collect(t.X1, t.X2), false);
          }
        } else {
          if (t.X1 > d.MidL()) {
            if (d.SubNearest(t.X1, t.X2, false)) d.Add(t.X1, 1, false);
          } else if (t.X2 > d.MidR()) {
            d.SubNearest(t.X1, t.X2, false);
            d.base++;
            d.Add(t.X1, d.Collect(t.X1, d.MidL())-1, true);
          } else {
            d.Add(t.X1, d.Collect(t.X1, t.X2), true);
          }
        }
        doit(t.idx);
      }
    };
    doit(N);

    auto it = d.mid;
    int ret = d.base;
    while (it->first >= R) { ret += it->second; --it; }
    ++it;
    while (it->first <= L) { ret += it->second; ++it; }
    cout << ret << endl;
  }
}
