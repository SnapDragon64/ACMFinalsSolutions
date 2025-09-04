#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int64_t X, Y, K, Tx, Ty, none = -1e18;
  while (cin >> X >> Y >> K >> Tx >> Ty) {
    vector<int64_t> gx(K), gy(K), gd(K), xh(X+1, none), yh(Y+1, none);
    vector<vector<int64_t>> xc(X+1), yc(Y+1);
    for (int i = 0; i < K; i++) {
      cin >> gx[i] >> gy[i] >> gd[i];
      xc[gx[i]].push_back(i);
      yc[gy[i]].push_back(i);
    }

    bool fail = false;
    int64_t ret = 0;
    for (int i = 0; i < K; i++) if (xh[gx[i]] == none) {
      vector<int64_t> xs, ys;
      int64_t minxv = 0, minyv = 0, txseen = 0, tyseen = 0;
      function<void(int,int64_t,int64_t)> doit = [&](int i, int64_t xv, int64_t yv) {
        if (xh[gx[i]] == none) {
          txseen |= (gx[i] == Tx);
          xs.push_back(gx[i]);
          minxv = min(minxv, xv);
          xh[gx[i]] = xv;
          for (auto j : xc[gx[i]]) doit(j, xv, yv + gd[j]-gd[i]);
        }
        if (yh[gy[i]] == none) {
          tyseen |= (gy[i] == Ty);
          ys.push_back(gy[i]);
          minyv = min(minyv, yv);
          yh[gy[i]] = yv;
          for (auto j : yc[gy[i]]) doit(j, xv + gd[j]-gd[i], yv);
        }
        if (xh[gx[i]] != xv || yh[gy[i]] != yv) fail = true;
      };
      doit(i, 0, 0);
      for (auto x : xs) xh[x] -= minxv;
      for (auto y : ys) yh[y] -= minyv;
      if (txseen) ret += xh[Tx];
      if (tyseen) ret += yh[Ty];
      if (txseen && tyseen) ret += gd[i]-xh[gx[i]]-yh[gy[i]];
    }
    for (int i = 0; i < K; i++) if (xh[gx[i]] + yh[gy[i]] > gd[i]) fail = true;

    if (fail) cout << "impossible" << endl; else cout << ret << endl;
  }
}
