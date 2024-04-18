#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int64_t N, C;
  while (cin >> N >> C) {
    vector<int64_t> T(N);
    for (auto& x : T) cin >> x;
    sort(T.begin(), T.end());

    vector<int64_t> tot(N+1, 1e18), cc(N, 1e18);
    tot[0] = cc[0] = 0;
    for (int64_t i = 0; i < N; i++) tot[i+1] = tot[i] + T[i];
    for (int64_t i = 1; i < C && i < N; i++) {
      for (int64_t j = i; j < cc.size(); j++) cc[j] = min(cc[j], cc[j-i] + T[i] + tot[i+1]);
    }
//for (int i = 0; i < N; i++) cerr << "cc[" << i << "] = " << cc[i] << endl;

    vector<int64_t> mnc(N+1), mxc(N+1);
    vector<vector<int64_t>> dyn(N+1);
    for (int64_t i = 0; i <= N; i++) mnc[i] = -(i/C) - (i==N);
    for (int64_t i = 0; i <= N; i++) mxc[i] = (N-i+C-1)/C-1;
    for (int64_t i = 0; i <= N; i++) dyn[i].resize(mxc[i]-mnc[i]+1, 1e18);
    dyn[0][0] = 0;
    for (int64_t i = 0; i < N; i++)
    for (int64_t ci = 0; ci < dyn[i].size(); ci++) {
      if (ci && dyn[i][ci] - dyn[i][0] >= cc[ci]) continue;
      int64_t c = mnc[i]+ci;
      for (int64_t j = min(N, i+C), extra = -1; j > i; j--, extra++) {
        int64_t c2 = c + extra;
        if (c2 > mxc[j]) break;
        dyn[j][c2-mnc[j]] = min(dyn[j][c2-mnc[j]], dyn[i][ci] + T[N-1-i] + tot[extra+1]);
      }
    }

    int64_t ret = 1e18;
    for (int64_t c = mnc[N]; c <= -1; c++) ret = min(ret, dyn[N][c-mnc[N]] + cc[-1-c]);
    cout << ret << endl;
  }
}
