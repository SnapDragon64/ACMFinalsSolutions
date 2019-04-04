#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<pair<int, int> > VPII;

// Given an undirected graph, returns a list of all biconnected components.
// Note that articulation vertices will belong to multiple components.
// Complexity: O(E)
VVI BiconnectedComponents(const VVI& succ) {
  VVI ret;
  VI idx(succ.size(), -1), stack;
  function<int(int)> doit = [&] (int x) {
    idx[x] = stack.size();
    stack.push_back(x);
    int low = idx[x];
    if (succ[x].size() == 0) {
      ret.emplace_back(1, x);
    }
    for (int i = 0; i < succ[x].size(); i++) {
      if (idx[succ[x][i]] == -1) {
        int curs = stack.size();
        int v = doit(succ[x][i]);
        if (v >= idx[x]) {
          ret.emplace_back(stack.begin()+curs, stack.end());
          ret.back().push_back(x);
          stack.resize(curs);
        } else {
          low = min(low, v);
        }
      } else {
        low = min(low, idx[succ[x][i]]);
      }
    }
    return low;
  };
  for (int x = 0; x < succ.size(); x++) if (idx[x] == -1) doit(x);
  return ret;
}

int main() {
  int N, M, V, W;
  while (cin >> N >> M) {
    VVI g(N), gi(N), d(N);
    for (int i = 0; i < M; i++) {
      cin >> V >> W;
      V--; W--;
      gi[V].push_back(g[W].size());
      gi[W].push_back(g[V].size());
      g[V].push_back(W);
      g[W].push_back(V);
      d[V].push_back(1);
      d[W].push_back(1);
    }

    VVI comps = BiconnectedComponents(g);

    VI seen(N);
    function<void(int,int)> clearD = [&] (int x, int i) {
      if (!d[x][i]) return;
      d[x][i] = 0;
      int y = g[x][i];
      if (++seen[y] > 3) return;
      for (int j = 0; j < g[y].size(); j++) if (g[y][j] != x) clearD(y, j);
    };

    for (auto const& comp : comps) if (comp.size() >= 3) {
      for (auto x : comp) for (int i = 0; i < g[x].size(); i++) clearD(x, i);
    }
    for (int x = 0; x < N; x++) for (int i = 0; i < g[x].size(); i++) {
      if (x < g[x][i]) swap(d[x][i], d[g[x][i]][gi[x][i]]);
    }

    seen = VI(N);
    for (int x = 0; x < N; x++) for (int i = 0; i < g[x].size(); i++) if (d[x][i]) {
      clearD(x, i);
      d[x][i] = 1;
    }

    VPII ret;
    for (int x = 0; x < N; x++) for (int i = 0; i < g[x].size(); i++) if (d[x][i]) {
      ret.emplace_back(x, g[x][i]);
    }
    sort(ret.begin(), ret.end());
    cout << ret.size() << endl;
    for (auto const& p : ret) cout << p.first+1 << ' ' << p.second+1 << endl;
  }
}
