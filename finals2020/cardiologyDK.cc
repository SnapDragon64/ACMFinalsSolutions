#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int64_t R, C;
  while (cin >> R >> C) {
    vector<int64_t> ret{1000000000};
    for (int64_t p = 0; p < C; p++) {
      int64_t sn = (p * (R*C-1) + (C-1)/2) / (C-1);
      int64_t i = -1, j = -1, s1 = -1, s2 = 0;
      for (int64_t x = 0, y = 0; x != j || y != i; s1++) {
        j = x;
        i = y;
        x = (y + p*R) % C;
        y = (y + p*R) / C;
      }
      if (i > 0 && j == 0 || i < R-1 && j == C-1) continue;
      for (int64_t x = C-1, y = R-1; x != j || y != i; s2++) {
        x = (y + p*R) % C;
        y = (y + p*R) / C;
      }
      int64_t d = min(abs((C-1)/2 - j), abs(C/2 - j)) +
                  min(abs((R-1)/2 - i), abs(R/2 - i));
      vector<int64_t> cur{d, p+1, i+1, j+1, max(s1, s2)};
      if (cur < ret) ret = cur;
    }
    cout << ret[1] << ' ' << ret[2] << ' ' << ret[3] << ' ' << ret[4] << endl;
  }
}
