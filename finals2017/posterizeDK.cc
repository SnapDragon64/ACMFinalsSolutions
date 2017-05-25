#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;

long long nv[256];
long long cumsum[257][257];

long long memo[256][256];
long long doit(int p, int k, int cur) {
  if (p == 256) return 0;
  if (k == 0) return 1e18;
  long long& ret = memo[p][k];
  if (ret != -1) return ret;
  ret = 1e18;

  for (int p2 = p+1; p2 <= 256; p2++) {
    while (cumsum[cur+1][p2] - cumsum[cur+1][p] <
           cumsum[cur][p2] - cumsum[cur][p]) {
      cur++;
    }
    ret = min(ret, cumsum[cur][p2] - cumsum[cur][p] + doit(p2, k-1, cur));
  }
  return ret;
}

int main() {
  int D, K, R, P;
  while (cin >> D >> K) {
    memset(nv, 0, sizeof(nv));
    for (int i = 0; i < D; i++) {
      cin >> R >> P;
      nv[R] += P;
    }

    for (int v = 0; v <= 256; v++)
    for (int i = 0; i < 256; i++) {
      cumsum[v][i+1] = cumsum[v][i] + (v-i)*(v-i)*nv[i];
    }
    memset(memo, -1, sizeof(memo));
    cout << doit(0, K, 0) << endl;
  }
}
