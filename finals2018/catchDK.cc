#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

struct Route {
  int B, E;
  long long S, T;
  double P, value;

  bool operator<(const Route& r) const {
    if (B != r.B) return B < r.B;
    return S < r.S;
  }
};

int main() {
  int M, N;
  long long K;

  while (cin >> M >> N >> K) {
    vector<Route> v(M+1);
    for (int i = 0; i < M; i++) {
      cin >> v[i].B >> v[i].E >> v[i].S >> v[i].T >> v[i].P;
      v[i].value = 0.0;
    }
    v[M].B = 1; v[M].S = K+1; v[M].value = 1.0;
    sort(v.begin(), v.end());

    vector<pair<long long, int>> ord(M+1);
    for (int i = 0; i <= M; i++) ord[i] = make_pair(-v[i].S, -i);
    sort(ord.begin(), ord.end());

    double ret = 0.0;
    for (int i = 0; i <= M; i++) if (-ord[i].first <= K) {
      int idx = -ord[i].second;
      Route& r = v[idx];
      r.value = 0.0;
      Route r2;

      r2.B = r.B;
      r2.S = r.S;
      auto it = upper_bound(v.begin(), v.end(), r2);
      if (it != v.end() && it->B == r.B) {
        r.value += (1.0-r.P) * it->value;  // Miss route.
      }

      r2.B = r.E;
      r2.S = r.T;
      it = upper_bound(v.begin(), v.end(), r2);
      if (it != v.end() && it->B == r.E) {
        r.value += r.P * it->value;  // Catch route.
      }

      if (idx < M && v[idx+1].B == v[idx].B) {
        r.value = max(r.value, v[idx+1].value);  // Don't try route at all.
      }

      if (r.B == 0) ret = max(ret, r.value);
    }

    printf("%0.8lf\n", ret);
  }
}
