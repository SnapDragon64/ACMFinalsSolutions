#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<int> L(N+1), R(N+1);
    for (int i = 1; i <= N; i++) cin >> L[i] >> R[i];

    vector<int> sz(N+1);
    function<int(int)> cnt = [&](int x) {
      if (x == 0) return 0;
      return sz[x] = 1 + cnt(L[x]) + cnt(R[x]);
    };
    cnt(1);

    for (int rep = 0; rep < 2; rep++) {
      bool fail = false;
      vector<int> ret(N);
      function<void(int,set<int>&)> doit = [&](int x, set<int>& i) {
        if (x == 0) return;
        if (sz[x] == 1) { ret[*i.begin()] = x; return; }
        int a = L[x], b = R[x], swp = 0;
        if (sz[a] == 0 && sz[b] > 0) { fail = true; return; }
        if (sz[a] <= sz[b]) { swap(a, b); swp = 1; }
        set<int> i2;
        auto it = --i.end();
        if (!swp) --it;
        for (int j = 0; j < sz[b]; j++) { i2.insert(*it); i.erase(it--); --it; }
        if (rep || sz[b]+!swp != sz[a]) ++it;
        ret[*it] = x;
        i.erase(it);
        doit(a, i);
        doit(b, i2);
      };

      set<int> idx;
      for (int i = 0; i < N; i++) idx.insert(i);
      doit(1, idx);
      if (fail) { cout << "impossible" << endl; break; }
      cout << ret[0];
      for (int i = 1; i < N; i++) cout << ' ' << ret[i];
      cout << endl;
    }
  }
}
