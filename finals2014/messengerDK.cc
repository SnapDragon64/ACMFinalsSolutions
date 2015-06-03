#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

#define EPS 1e-9

struct Point {
  double x, y;
  Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}
  Point operator-(const Point& p) const {return Point(x-p.x, y-p.y);}
  Point operator+(const Point& p) const {return Point(x+p.x, y+p.y);}
  Point operator*(double c) const {return Point(x*c, y*c);}
  Point operator/(double c) const {return Point(x/c, y/c);}
  double len() {return hypot(x, y);}
};

double dotp(const Point& a, const Point& b) {
  return a.x*b.x + a.y*b.y;
}
double crossp(const Point& a, const Point& b) {
  return a.x*b.y - a.y*b.x;
}

double ClosestApproach(const Point& a, const Point& b, const Point& p) {
  if (dotp(b-a, p-a) <= 0) return (p-a).len();
  if (dotp(a-b, p-b) <= 0) return (p-b).len();
  return abs(crossp(b-a, p-a)) / (b-a).len();
}

main() {
  int N1, N2;
  while (cin >> N1) {
    vector<Point> P1(N1);
    for (int i = 0; i < N1; i++) cin >> P1[i].x >> P1[i].y;
    vector<double> T1(N1);
    for (int i = 1; i < N1; i++) T1[i] = T1[i-1] + (P1[i]-P1[i-1]).len();
    cin >> N2;
    vector<Point> P2(N2);
    for (int i = 0; i < N2; i++) cin >> P2[i].x >> P2[i].y;
    vector<double> T2(N2);
    for (int i = 1; i < N2; i++) T2[i] = T2[i-1] + (P2[i]-P2[i-1]).len();

    if ((P2.back()-P1[0]).len() > T2.back()+EPS) {
      printf("impossible\n");
      continue;
    }

    double dlo = 0.0, dhi = T2.back();
    while (dhi-dlo > EPS) {
      double d = (dhi+dlo)/2;
      int i1, i2;
      bool success = false;
      for (i1 = 0, i2 = 0; i1+1 < N1 && i2+1 < N2; ) {
        if (T2[i2+1] < T1[i1] + d) {
          i2++;
          continue;
        }
        double t1 = max(T1[i1]+d, T2[i2]);
        double t2 = min(T1[i1+1]+d, T2[i2+1]);
        assert(t1 < t2+EPS);
        Point v1 = P1[i1+1]-P1[i1];
        Point v2 = P2[i2+1]-P2[i2];
        v1 = v1 / v1.len(); v2 = v2 / v2.len();
        Point p1 = P1[i1] + v1 * (t1-d-T1[i1]);
        Point p2 = P2[i2] + v2 * (t1-T2[i2]);
        if (ClosestApproach(p1, p1+(v1-v2)*(t2-t1), p2) <= d + EPS) {
          success = true;
          break;
        }
        if (T1[i1+1]+d < T2[i2+1]) i1++; else i2++;
      }
      if (success) dhi = d; else dlo = d;
    }

    printf("%.6lf\n", (dhi+dlo)/2);
  }
}
