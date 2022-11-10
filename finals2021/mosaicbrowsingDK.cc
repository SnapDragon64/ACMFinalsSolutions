#include <bitset>
#include <iostream>
#include <vector>
using namespace std;

typedef bitset<1000> bs;

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int PY, PX, QY, QX, PC, QC;
  while (cin >> PY >> PX) {
    vector<bs> g(PX);
    vector<vector<bs>> cg(101, vector<bs>(PX));
    for (int y = 0; y < PY; y++)
    for (int x = 0; x < PX; x++) {
      cin >> PC;
      if (!PC) continue;
      g[PX-1-x].set(PY-1-y);
      cg[PC][PX-1-x].set(PY-1-y);
    }
    for (auto& v : cg) for (auto& b : v) b = ~b;

    cin >> QY >> QX;
    vector<bs> fail(QX);
    vector<vector<bs>> gg(101, vector<bs>(PX));
    vector<vector<int>> cookie(101, vector<int>(PX, -1));
    for (int y = 0; y < QY; y++) {
      for (int x = 0; x < QX; x++) {
        cin >> QC;
        for (int px = max((PX-1)-x, 0); px < PX; px++) {
          int fx = x-(PX-1)+px;
          if (fx > QX-PX) break;
          if (cookie[QC][px] != y) {
            cookie[QC][px] = y;
            if (y < PY) {
              gg[QC][px] = (g[px] & cg[QC][px]) >> (PY-1)-y;
            } else {
              gg[QC][px] = (g[px] & cg[QC][px]) << y-(PY-1);
            }
          }
          fail[x-(PX-1)+px] |= gg[QC][px];
        }
      }
    }

    vector<pair<int, int>> v;
    for (int y = 0; y <= QY-PY; y++)
    for (int x = 0; x <= QX-PX; x++) if (!fail[x][y]) {
      v.push_back({x, y});
    }
    cout << v.size() << endl;
    for (auto [x, y] : v) cout << y+1 << ' ' << x+1 << endl;
  }
}
