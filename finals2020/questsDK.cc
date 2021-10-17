#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, V, C;
  while (cin >> N >> V >> C) {
    vector<pair<int, int>> DX(N);
    for (auto& [d, x] : DX) cin >> x >> d;

    vector<bool> dyn(N * 2000 + 2001);
    dyn[0] = true;
    for (auto& [d, x] : DX) d = (d*V-1)/C + x;
    sort(DX.begin(), DX.end());
    int totx = 0;
    for (auto [d, x] : DX) {
      for (int i = min(totx, d-x); i >= 0; i--) if (dyn[i]) dyn[i+x] = true;
      totx += x;
    }

    int64_t ret = 0;
    for (auto [_, x] : DX) ret += x;
    for (int64_t i = totx; ; i--) if (dyn[i]) {
      ret += i * (C-1);
      break;
    }

    cout << ret << endl;
  }
}

