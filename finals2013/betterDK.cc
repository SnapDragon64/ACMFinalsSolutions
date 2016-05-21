#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

main() {
  int prob = 1;
  double X, P;
  while (cin >> X >> P) {
    X /= 100.0; P /= 100.0;
    double ret = 0.0;
    if (P > 0.0)
    for (int down = 1; down <= 2000; down++)
    for (int up = 1; up <= 2000; up++) {
      double ratio = (1-P)/P;
      double probup = (1 - pow(ratio, -down)) /
                      (pow(ratio, up) - pow(ratio, -down));
      ret = max(ret, probup * up - (1-probup) * down * (1-X));
    }
    printf("%.8lf\n", ret);
  }
}
