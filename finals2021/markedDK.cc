#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int K, M;
  while (cin >> K >> M) {
    vector<int> col(M);
    for (int i = 0; i < M; i++) cin >> col[i];

    double hands = 0.0, matched = 0.0;
    vector<int> v(K);
    vector<double> fact(K);
    function<void(int,int)> rec = [&] (int i, int x) {
      if (i < K) {
        for (v[i] = x; v[i] < M; v[i]++) rec(i+1, v[i]);
        return;
      }
      double h = 1.0, m = 0.0;
      for (int i = 0, last = -1, n = 1; i < K; i++) {
        if (v[i] == last) n++; else n = 1;
        last = v[i];
        fact[i] = double(col[v[i]]-(n-1)) / n;
        h *= fact[i];
      }
      if (h == 0.0) return;
      for (int i = 0; i < K; i++) if (i == K-1 || v[i] != v[i+1]) {
        m += h/fact[i];
      }
      for (int i = 1; i <= K-1; i++) m *= i;
      hands += h;
      matched += min(h, m);
    };
    rec(0, 0);
    printf("%.12lf\n", matched/hands);
  }
}
