#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, K, T;
  while (cin >> N >> K >> T) {
    int sz = pow(3.0, K);
    vector<int64_t> v(sz);
    for (int i = 0; i < T; i++) {
      char ch;
      int x = 0;
      for (int j = 0; j < K; j++) { cin >> ch; x = (x*3) + (ch-'A')/4; }
      cin >> v[x];
    }

    vector<int64_t> slice(3*sz);
    vector<vector<short>> ortho(sz), sum(sz, vector<short>(sz));
    vector<short> inv(sz);
    function<void(int,int,int,int,int)> doit = [&](int k, int x, int y, int z, int prod) {
      if (k == K) {
        slice[x*3+prod%3] += v[y];
        sum[x][y] = z;
        if (prod%3 == 0) ortho[x].push_back(y);
        if (z == 0) inv[x] = y;
        return;
      }
      for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) {
        doit(k+1, x*3+i, y*3+j, z*3+(i+j)%3, prod+i*j);
      }
    };
    doit(0, 0, 0, 0, 0);

    vector<int> northo(sz);
    int totOrtho = 0;
    for (int x = 0, i = 0; x < sz; x++, i += 3) {
      for (int64_t pw = 2; slice[i]%pw == 0 && slice[i+1]%pw == 0 && slice[i+2]%pw == 0; pw *= 2) {
        northo[x]++;
      }
      totOrtho += northo[x];
    }

    int nzero = (totOrtho - (sz/3)*N) / (sz-sz/3);
    vector<int> poss;
    for (int x = 1; x < sz; x++) {
      if (x > inv[x]) continue;
      int tot = 0;
      for (auto y : ortho[x]) tot += northo[y];
      for (int n = nzero; tot != (sz/3)*n + (sz/9)*(N-n); n++) {
        poss.push_back(x);
        poss.push_back(inv[x]);
      }
    }

    for (int i = 0; i < nzero; i++) cout << string(K, '0') << endl;
    for (auto& x : v) x >>= nzero;
    vector<int> cur(N-nzero);
    function<bool(int,const vector<int64_t>&)> rec = [&](int i, const vector<int64_t>& oldv) {
      if (i == poss.size()) {
        if (oldv[0] != 1) return false;
        for (auto x : cur) {
          string s;
          for (int i = 0; i < K; i++, x /= 3) s += "0+-"[x%3];
          reverse(s.begin(), s.end());
          cout << s << endl;
        }
        return true;
      }
      for (int j = i; j < i+2; j++) {
        int& x = cur[i/2];
        x = poss[j];
        vector<int64_t> newv = oldv;
        bool valid = true;
        for (int a = 0; a < sz; a++) {
          int b = sum[x][a];
          int c = sum[x][b];
          if (a > b || a > c) continue;
          if ((oldv[a]%2) ^ (oldv[b]%2) ^ (oldv[c]%2)) { valid = false; break; }
          newv[a] = (oldv[a] + oldv[b] - oldv[c]) / 2;
          newv[b] = (oldv[b] + oldv[c] - oldv[a]) / 2;
          newv[c] = (oldv[c] + oldv[a] - oldv[b]) / 2;
          if (newv[a] < 0 || newv[b] < 0 || newv[c] < 0) { valid = false; break; }
        }
        if (valid && rec(i+2, newv)) return true;
      }
      return false;
    };
    rec(0, v);
  }
}
