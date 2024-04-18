#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

// Given an undirected graph, returns a list of all biconnected components and the edge counts in them.
// Note that articulation vertices will belong to multiple components.
// Complexity: O(E)
pair<VVI, VI> BiconnectedComponents(const VVI& g) {
  VVI ret;
  VI idx(g.size(), -1), stack, rete;
  int ne = 0;
  function<int(int,int)> doit = [&](int x, int prev) {
    idx[x] = stack.size();
    stack.push_back(x);
    int low = idx[x];
    if (g[x].empty()) { ret.emplace_back(1, x); rete.push_back(0); }
    for (auto y : g[x]) {
      int curne = ne;
      if (y != prev && idx[y] <= idx[x]) ne++;
      if (idx[y] == -1) {
        int curs = stack.size();
        int v = doit(y, x);
        if (v >= idx[x]) {
          ret.emplace_back(stack.begin()+curs, stack.end());
          ret.back().push_back(x);
          stack.resize(curs);
          rete.push_back(ne - curne);
          ne = curne;
        } else {
          low = min(low, v);
        }
      } else {
        low = min(low, idx[y]);
      }
    }
    return low;
  };
  for (int x = 0; x < g.size(); x++) if (idx[x] == -1) doit(x, -1);
  return {ret, rete};
}

bool IsRotation(const VI& a, const VI& b) {
  // Could use fancy string algorithms for this but I prefer cheating with hash.
  auto withMod = [&](int64_t mod, int64_t x) {
    int64_t av = 0, bv = 0, c = 1;
    for (int64_t i = 0, pw = 1; i < a.size(); i++, pw = (pw * x) % mod) av = (av + pw * a[i]) % mod;
    for (int64_t i = 0, pw = 1; i < b.size(); i++, pw = (pw * x) % mod) bv = (bv + pw * b[i]) % mod;
    for (int64_t i = 0; i < a.size(); i++) c = (c * x) % mod;
    c = 1+mod-c;
    for (int64_t i = a.size()-1; i >= 0; i--) {
      if ((av-bv) % mod == 0) return true;
      av = (av * x + c * a[i]) % mod;
    }
    return false;
  };
  if (!withMod(1000000007, 2)) return false;
  if (!withMod(1234567891, 42)) return false;
  return withMod(rand()%1000000000 + 500000000, rand()%1000 + 50);
}

int main() {
  int N, M, X, Y;
  while (cin >> N >> M) {
    VI S(N), E(N);
    for (int i = 0; i < N; i++) cin >> S[i] >> E[i];
    VVI g(N);
    for (int i = 0; i < M; i++) {
      cin >> X >> Y;
      X--; Y--;
      g[X].push_back(Y);
      g[Y].push_back(X);
    }

    // Find BCCs.  Identify which are cycles, and which have any even-length cycle at all.
    auto [comp, compe] = BiconnectedComponents(g);
    VI cyc(comp.size()), hasEven(comp.size());
    for (int i = 0; i < comp.size(); i++) {
      if (comp[i].size() <= 2) { comp[i].clear(); continue; }  // Ignore bridges.
      cyc[i] = (compe[i] == comp[i].size());
      // Note that if there's more than one cycle in a component, we can make an even cycle.
      hasEven[i] = (comp[i].size() % 2 == 0 || !cyc[i]);
    }

    // Merge all components with shared articulation vertices.
    VI rootComp(N, -1);
    for (int i = comp.size()-1; i >= 0; i--) if (comp[i].size()) {
      int rc = i;
      for (auto x : comp[i]) rc = max(rc, rootComp[x]);
      if (rc > i) {
        cyc[rc] = false;
        hasEven[rc] |= hasEven[i];
        for (auto x : comp[i]) if (rootComp[x] != rc) {
          assert(rootComp[x] == -1);  // BCC does a postorder traversal; no need for union-find.
          rootComp[x] = rc;
          comp[rc].push_back(x);
        }
        comp[i].clear();
      } else {
        for (auto x : comp[i]) rootComp[x] = rc;
      }
    }

    VI aTot(1000001), aNext(1000001);
    for (int i = 0; i < N; i++) if (rootComp[i] == -1 && S[i] != E[i]) goto fail;  // Isolated points can't move.
    for (int i = 0; i < comp.size(); i++) if (comp[i].size()) {
      //cerr << "component size=" << comp[i].size() << " cyc=" << cyc[i] << " hasEven=" << hasEven[i] << endl;
      if (cyc[i]) {
        // This component is a bare-bones cycle; we can only do rotations.
        VI s, e;
        for (auto x : comp[i]) { s.push_back(S[x]); e.push_back(E[x]); }
        if (!IsRotation(s, e)) goto fail;
      } else {
        // Animal numbers must match up in this component.
        bool needsEven = !hasEven[i];
        for (auto x : comp[i]) aTot[S[x]]++;
        for (auto x : comp[i]) if (aTot[S[x]] >= 2) needsEven = false;
        for (auto x : comp[i]) aTot[E[x]]--;
        for (auto x : comp[i]) if (aTot[S[x]]) goto fail;

        if (needsEven) {
          // This component has no even-length cycles, and no duplicate animals, so only even permutations
          // are solvable.
          for (auto x : comp[i]) aNext[S[x]] = E[x];
          int parity = 0;
          for (auto x : comp[i]) if (aNext[S[x]] != -1) {
            int cyc = 0;
            for (int a = S[x]; aNext[a] != -1; cyc++) {
              int tmp = a;
              a = aNext[a];
              aNext[tmp] = -1;
            }
            parity += cyc-1;
          }
          if (parity % 2) goto fail;
        }
      }
    }

    cout << "possible" << endl;
    continue;
fail:
    cout << "impossible" << endl;
  }
}
