#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct RangeOp {
  vector<int> v;
  int s;

  RangeOp(int n) {
    for (s = 1; s < n; s <<= 1)
      ;
    v = vector<int>(s*2);
  }

  void set(int x, int val) {
    v[x += s] = val;
    while (x >>= 1) v[x] = v[2*x] + v[2*x+1];
  }

  int calc(int a, int b) {
    int ret = 0;
    for (a += s, b += s; a < b; a >>= 1, b >>= 1) {
      if (a&1) ret += v[a++];
      if (b&1) ret += v[--b];
    }
    return ret;
  }
};

int main() {
  ios::sync_with_stdio(false);
  int R, C;
  string line;
  while (getline(cin, line)) {
    sscanf(line.c_str(), "%d %d", &R, &C);
    R = 2*R-1; C = 2*C-1;
    vector<string> v(R);
    for (int i = 0; i < R; i++) {
      getline(cin, v[i]);
      v[i] += string(C-v[i].size(), ' ');
    }

    long long ret = 0;
    for (int swp = 0; swp < 2; swp++) {
      int sx = (swp == 0 || swp == 1 && R%4 == 1) ? 0 : 2;
      vector<vector<int>> ur(R, vector<int>(C)), ul(R, vector<int>(C));
      for (int x = sx; x < C+R; x += 4)
      for (int y = max(2, x-(C-3)); y < R && x-y >= 0; y += 2) {
        if (v[y-1][x-y+1] != ' ') ur[y][x-y] = ur[y-2][x-y+2]+1;
      }
      for (int x = sx-R/4*4; x < C; x += 4)
      for (int y = max(2, 4-x); y < R && x+y < C; y += 2) {
        if (v[y-1][x+y-1] != ' ') ul[y][x+y] = ul[y-2][x+y-2]+1;
      }
      vector<vector<int>> event(C);
      for (int y = 0; y < R; y += 2) {
        RangeOp rop(C/4+1);
        for (int x = sx^(y%4), lx = x; x < C; x += 4) {
          if (x && v[y][x-1] != ' ') {
            ret += rop.calc(max(lx, x-ul[y][x]*4)/4, x/4);
          } else {
            lx = x;
          }
          if (ur[y][x]) {
            if (x+4*ur[y][x] < C) event[x+4*ur[y][x]].push_back(x);
            rop.set(x/4, 1);
          }
          for (auto ex : event[x]) {
            rop.set(ex/4, 0);
          }
          event[x].clear();
        }
      }
      reverse(v.begin(), v.end());
    }
    cout << ret << endl;
  }
}
