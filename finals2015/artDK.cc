#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

main() {
  int p, a, b, c, d, n;
  while (cin >> p >> a >> b >> c >> d >> n) {
    vector<double> v(n+1);
    for (double i = 1; i <= n; i++) {
      v[i] = p*(sin(a*i + b) + cos(c*i + d) + 2);
    }
    double mx = v[1], ret = 0.0;
    for (int i = 1; i <= n; i++) {
      if (v[i] > mx) {
        mx = v[i];
      } else {
        ret = max(ret, mx-v[i]);
      }
    }
    printf("%.6lf\n", ret);
  }
}
