#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int M, N, Q;
  while (cin >> M >> N >> Q) {
    vector<vector<int>> nd(1);
    int mx = 0;
    for (int i = 1; i <= N; i++) {
      char ch;
      int x, y, z;
      cin >> ch >> x >> y >> z;
      mx = max<int>({mx, x, y, z});
      if (ch == 'S') nd.push_back({x, 0, y, z}); else nd.push_back({x, y, z, 0});
    }
    vector<int> oin(mx+1), oout(mx+1);
    for (int i = 1; i <= N; i++) oin[nd[i][0]] = oin[nd[i][1]] = oout[nd[i][2]] = oout[nd[i][3]] = i;

    vector<int> osz(mx+1, -1);
    osz[0] = 0;
    function<void(int,int)> rec = [&](int x, int sz) {
      osz[x] = sz;
      if (oin[x] == 0) return;
      auto const& v = nd[oin[x]];
      if (osz[v[0]] == -1 || osz[v[1]] == -1) return;
      if (v[1]) {
        rec(v[2], osz[v[0]]+osz[v[1]]);
      } else {
        rec(v[2], (osz[v[0]]+1)/2);
        rec(v[3], (osz[v[0]]  )/2);
      }
    };
    rec(1, M);
    for (int i = 2; i <= mx; i++) if (!oout[i]) rec(i, 0);

    for (int q = 0; q < Q; q++) {
      int x, k;
      cin >> x >> k;
      if (k > osz[x]) { cout << "none" << endl; continue; }
      while (x != 1) {
        auto const& v = nd[oout[x]];
        if (v[1]) {
          int sz = min(osz[v[0]], osz[v[1]]);
          if (k <= 2*sz) {
            x = v[!(k%2)];
            k = (k+1)/2;
          } else {
            x = v[osz[v[1]] > osz[v[0]]];
            k -= sz;
          }
        } else {
          k = 2*k - (v[2] == x);
          x = v[0];
        }
      }
      cout << k << endl;
    }
  }
}
