#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, T;
  while (cin >> N >> T) {
    double mn = -1e9, mx = 1e9;
    vector<double> D(N), S(N);
    for (int i = 0; i < N; i++) {
      cin >> D[i] >> S[i];
      mn = max(mn, -S[i]);
    }

    while (mx-mn > 1e-9) {
      double c = (mx+mn)/2, t = 0.0;
      for (int i = 0; i < N; i++) t += D[i] / (S[i] + c);
      if (t > T) mn = c; else mx = c;
    }

    printf("%.9lf\n", (mx+mn)/2);
  }
}
