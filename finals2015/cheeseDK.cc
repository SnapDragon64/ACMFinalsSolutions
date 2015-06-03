#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

#define PI 3.14159265358979323846
#define EPS 1e-8

double slice(double R, double z, double h) {
  h = max(0.0, min(2*R, h-(z-R)));
  return PI*h*h*(R-h/3);
}

int main() {
  int N, S;
  while (cin >> N >> S) {
    double vol = 1e15;
    vector<double> vr(N), vx(N), vy(N), vz(N);
    for (int i = 0; i < N; i++) {
      cin >> vr[i] >> vx[i] >> vy[i] >> vz[i];
      vol -= 4.0/3.0*PI*vr[i]*vr[i]*vr[i];
    }
    double last = 0.0;
    for (int i = 0; i < S; i++) {
      double goal = vol*(i+1)/S;
      double lo = last, hi = 100000.0;
      while (hi-lo > EPS) {
        double h = (hi+lo)/2, cur = h*1e10;
        for (int i = 0; i < N; i++) cur -= slice(vr[i], vz[i], h);
        if (cur > goal) hi = h; else lo = h;
      }
      double h = (hi+lo)/2;
      printf("%.9lf\n", (h-last)/1000);
      last = h;
    }
  }
}
