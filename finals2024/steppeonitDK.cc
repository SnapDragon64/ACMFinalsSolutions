#include <algorithm>
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

int main() {
  int N, F, V, T;
  while (cin >> N >> F) {
    vector<vector<pair<int, int>>> g(N+1);
    for (int i = 2; i <= N; i++) {
      cin >> V >> T;
      g[V].push_back({i, T});
    }

    int lo = 0, hi = N*10000;
    while (lo < hi) {
      int mid = (lo+hi)/2;
      function<tuple<int,int,int>(int)> rec = [&](int x) {
        int mxf = -1, mxt = 0, totf = 0;
        for (auto [y, d] : g[x]) {
          auto [df, dt, nf] = rec(y);
          mxf = max(mxf, df-d);
          totf += nf;
          if (dt > -1) {
            if (dt+d > mid) {
              totf++;
              mxf = max(mxf, mid-d);
            } else {
              mxt = max(mxt, dt+d);
            }
          }
        }
        if (mxf >= mxt) mxt = -1;
        return make_tuple(mxf, mxt, totf);
      };
      auto [df, dt, nf] = rec(1);
      if (dt > -1) nf++;
      if (nf > F) lo = mid+1; else hi = mid;
    }
    cout << lo << endl;
  }
}
