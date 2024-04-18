#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int L, B;
  while (cin >> L >> B) {
    vector<vector<int>> lb(L), bl(B);
    vector<int> ls(L);
    for (int i = 0; i < L; i++) {
      char ch;
      cin >> ch;
      ls[i] = string("RGB").find(ch);
    }
    for (int i = 0; i < B; i++) {
      int K;
      cin >> K;
      bl[i].resize(K);
      for (auto& x : bl[i]) {
        cin >> x; x--;
        lb[x].push_back(i);
      }
    }

    int ret = 0;
    vector<int> push(B, -1);
    for (int j = 0; j < L; j++) if (lb[j].empty() && ls[j] != 0) goto fail;
    for (int i = 0; i < B; i++) if (push[i] == -1 && bl[i].size()) {
      int best = 1e9;
      function<int(int,int,int)> rec = [&](int i, int p, int cookie) -> int {
        if (push[i] >= cookie) return push[i] == cookie+p ? 0 : 1e9;
        push[i] = cookie+p;
        int ret = p;
        for (auto j : bl[i]) if (lb[j].size() == 2) {
          int k = lb[j][0] ^ lb[j][1] ^ i;  // Ooh, I'm so clever.
          ret += rec(k, (12-ls[j]-p)%3, cookie);
          if (ret >= 1e9) return 1e9;
        } else {
          if ((ls[j] + p) % 3 != 0) return 1e9;
        }
        return ret;
      };
      for (int p = 0; p < 3; p++) best = min(best, rec(i, p, p*3));
      if (best == 1e9) goto fail;
      ret += best;
    }

    cout << ret << endl;
    continue;
fail:
    cout << "impossible" << endl;
  }
}
