#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace std;

long double Comb(int a, int b) {
  #define COMB__MAXA 2000
  #define COMB__MAXB 2000
  if (b > a || b < 0) return 0;
  if (!a) return 1;
  static long double combmemo[COMB__MAXA+1][COMB__MAXB+1];
  long double& ret = combmemo[a][b];
  if (!ret) ret = Comb(a-1, b-1)+Comb(a-1,b);
  return ret;
}

// Probability of first person getting g gems if d are distributed to n people.
inline long double finalProb(int n, int d, int g) {
  if (n == 1) return d == g;
  return Comb(n+d-g-2, d-g) / Comb(n+d-1, d);
}

map<int, pair<double, double>> m[1001][1001];

void add(int n, int d, int mx, double p, double e) {
  if (p < 1e-10) return;
  if (mx * n < d) return;
  if (mx > d) mx = d;
  if (mx < 0) mx = 0;
  pair<double, double>& ret = m[n][mx][d];
  ret.first += p; ret.second += e;
}

int main() {
  int N, D, R;
  cin >> N >> D >> R;
  add(N, D, D, 1.0, 0.0);
  for (int n = N; n >= 0; n--)
  for (int mx = D; mx >= 0; mx--)
  for (auto it : m[n][mx]) {
    int d = it.first;
    long double p = it.second.first, e = it.second.second;
    if (mx > 1 || mx == 1 && d == 0) add(n, d, mx-1, p, e);
    long double p2 = p;
    for (int rep = 1, n2 = n, d2 = d, eg = 0; n2 > 0 && d2 >= mx; rep++, n2--, d2 -= mx) {
      p2 *= finalProb(n2, d2, mx) * n2 / rep;
      if (N-n2 < R) eg += mx;
      if (mx > 0 || n2-1 == 0) add(n2-1, d2-mx, mx-1, p2, p2*(e/p+eg));
    }
  }
  long double tot = 0.0, ret = 0.0;
  for (auto it : m[0][0]) { tot += it.second.first; ret += it.second.second; }
  printf("%.7lf\n", (double)(ret/tot)+R);
}
