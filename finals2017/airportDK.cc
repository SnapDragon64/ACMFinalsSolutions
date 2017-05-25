#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
  double x, y;
  Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}
  Point operator-(const Point &p) const { return Point(x-p.x, y-p.y); }
  Point operator*(double c) const { return Point(x*c, y*c); }
  Point operator/(double c) const { return Point(x/c, y/c); }
  double len() const { return hypot(x, y); }
};

double CrossP(const Point& a, const Point& b) {
  return a.x*b.y - a.y*b.x;
}
double DotP(const Point& a, const Point& b) {
  return a.x*b.x + a.y*b.y;
}
Point Intersection(const Point& a1, const Point& a2,
                  const Point& b1, const Point& b2) {
  double v1 = CrossP(b2-b1, a1-b1);
  double v2 = CrossP(b2-b1, a2-b1);
  return (a1*v2 - a2*v1) / (v2-v1);
}

vector<Point> P;

int main() {
  int N;
  while (cin >> N) {
    P = vector<Point>(N);
    for (int i = 0; i < N; i++) cin >> P[i].x >> P[i].y;

    // Eliminate colinear edges.
    for (int i = 0; i < N; i++) {
      if (CrossP(P[i]-P[(i+N-1)%N], P[(i+1)%N]-P[i]) == 0) {
        P.erase(P.begin() + i);
        i--; N--;
      }
    }

    // Make sure vertices are in CCL order.
    double ar = 0.0;
    for (int i = 0; i < N; i++) ar += CrossP(P[i], P[(i+1)%N]);
    if (ar < 0.0) reverse(P.begin(), P.end());

    double ret = 0.0;
    for (int i = 0; i < N; i++)
    for (int j = i+1; j < N; j++) {
      Point a = P[i], b = P[j];
      if (CrossP(b-a, P[(i+N-1)%N]-a) * CrossP(b-a, P[(i+1)%N]-a) > 0 &&
          CrossP(a-P[(i+N-1)%N], P[(i+1)%N]-a) > 0) {
        // Normally an incident vertex doesn't matter... unless it's vertex a.
        continue;
      }
      double alen = 1e50, blen = 1e50;
//cout << a.x << ',' << a.y << " - " << b.x << ',' << b.y << ":" << endl;
      for (int k = 0; k < N; k++) {
        Point p0 = P[(k+N-1)%N], p1 = P[k], p2 = P[(k+1)%N], p3 = P[(k+2)%N];
        if (CrossP(b-a, p1-a) == 0 && CrossP(b-a, p2-a) == 0) {
          // p1-p2 colinear with a-b.
          double dp1 = DotP(b-a, p1-a), dp2 = DotP(b-a, p2-a);
          if (dp2 <= 0 && dp2 <= dp1 && CrossP(b-a, p3-a) < 0) {
//cout << " alen p2: " << p2.x << ',' << p2.y << endl;
            alen = min(alen, DotP(a-b, p2-a));
          }
          if (dp1 <= 0 && dp1 <= dp2 && CrossP(b-a, p0-a) > 0) {
//cout << " alen p1: " << p1.x << ',' << p1.y << endl;
            alen = min(alen, DotP(a-b, p1-a));
          }
          if (dp2 >= 0 && dp2 >= dp1 && CrossP(b-a, p3-a) > 0) {
//cout << " blen p2: " << p2.x << ',' << p2.y << endl;
            blen = min(blen, DotP(b-a, p2-a));
          }
          if (dp1 >= 0 && dp1 >= dp2 && CrossP(b-a, p0-a) < 0) {
//cout << " blen p1: " << p1.x << ',' << p1.y << endl;
            blen = min(blen, DotP(b-a, p1-a));
          }
        } else if (CrossP(p2-p1, a-p1) >= 0 && CrossP(b-a, p2-a) < 0 &&
                   (CrossP(b-a, p1-a) > 0 ||
                    CrossP(b-a, p1-a) == 0 && CrossP(b-a, p0-a) > 0)) {
          Point ip = Intersection(p1, p2, a, b);
//cout << " alen ip: " << ip.x << ',' << ip.y << endl;
          alen = min(alen, DotP(a-b, ip-a));
        } else if (CrossP(p2-p1, a-p1) >= 0 && CrossP(b-a, p2-a) > 0 &&
                   (CrossP(b-a, p1-a) < 0 ||
                    CrossP(b-a, p1-a) == 0 && CrossP(b-a, p0-a) < 0)) {
          Point ip = Intersection(p1, p2, a, b);
//cout << " blen ip: " << ip.x << ',' << ip.y << endl;
          blen = min(blen, DotP(b-a, ip-a));
        }
      }
//cout << "  " << (alen+blen)/(b-a).len() << endl;
      ret = max(ret, (alen+blen)/(b-a).len());
    }

    printf("%.9lf\n", ret);
  }
}
