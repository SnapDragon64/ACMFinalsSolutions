#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

struct Tile {
  int P, S, Idx;
  bool operator<(const Tile& t) const { return S < t.S; }
};

int main() {
  int N;
  while (cin >> N) {
    vector<multiset<Tile>> v[2];
    for (int row = 0; row < 2; row++) {
      vector<Tile> T(N);
      for (int i = 0; i < N; i++) cin >> T[i].P;
      for (int i = 0; i < N; i++) cin >> T[i].S;
      for (int i = 0; i < N; i++) T[i].Idx = i+1;
      sort(T.begin(), T.end(), [] (const Tile& a, const Tile& b) -> bool { return a.P < b.P; });
      for (int i = 0; i < N; i++) {
        if (i == 0 || T[i-1].P != T[i].P) v[row].emplace_back();
        v[row].back().insert(T[i]);
      }
    }

    vector<int> ret[2];
    for (int i0 = 0, i1 = 0; ret[0].size() < N; ) {
      if (v[0][i0].size() < v[1][i1].size()) {
        for (auto const& t : v[0][i0]) {
          auto it = v[1][i1].upper_bound(Tile{t.P, t.S-1, t.Idx});
          if (it == v[1][i1].begin()) goto fail;
          --it;
          ret[0].push_back(t.Idx);
          ret[1].push_back(it->Idx);
          v[1][i1].erase(it);
        }
        i0++;
      } else {
        for (auto const& t : v[1][i1]) {
          auto it = v[0][i0].upper_bound(t);
          if (it == v[0][i0].end()) goto fail;
          ret[1].push_back(t.Idx);
          ret[0].push_back(it->Idx);
          v[0][i0].erase(it);
        }
        i1++;
        if (v[0][i0].size() == 0) i0++;
      }
    }

    for (int row = 0; row < 2; row++) {
      for (int i = 0; i < ret[row].size(); i++) {
        if (i) cout << ' ';
        cout << ret[row][i];
      }
      cout << endl;
    }
    continue;
fail:
    cout << "impossible" << endl;
  }
}
