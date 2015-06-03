#include <algorithm>
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

Point intersect(const Point& l1a, const Point& l1b,
                const Point& l2a, const Point& l2b) {
  double cp1 = crossp(l1b-l1a, l2a-l1a);
  double cp2 = crossp(l1b-l1a, l2b-l1a);
  return (l2a * cp2 - l2b * cp1) / (cp2 - cp1);
}

vector<Point> ClipPoly(const vector<Point>& p, const Point& a, const Point& b) {
  vector<Point> ret;
  int i;
  for (i = 0; i < p.size(); i++) {
    if (crossp(b-a, p[i]-a) < 0) break;
  }
  if (i == p.size()) return ret;
  for (i = 0; i < p.size(); i++) {
    const Point& p1 = p[i];
    const Point& p2 = p[(i+1)%p.size()];
    if (crossp(b-a, p1-a) >= 0 && crossp(b-a, p2-a) < 0) {
      ret.push_back(intersect(p1, p2, a, b));
      break;
    }
  }
  if (i == p.size()) return p;
  for (int j = 1; ; j++) {
    const Point& p1 = p[(i+j)%p.size()];
    const Point& p2 = p[(i+j+1)%p.size()];
    ret.push_back(p1);
    if (crossp(b-a, p2-a) >= 0) {
      ret.push_back(intersect(p1, p2, a, b));
      break;
    }
  }
  return ret;
}

double PolyArea(const vector<Point>& p) {
  double ret = 0.0;
  for (int i = 0; i < p.size(); i++) ret -= crossp(p[i], p[(i+1)%p.size()]);
  return ret/2;
}

double calc(vector<Point> p1, const vector<Point>& p2, Point v, double t) {
  for (int j = 0; j < p1.size(); j++) p1[j] = p1[j] + v*t;
  for (int j = 0; j < p2.size(); j++) {
    p1 = ClipPoly(p1, p2[j], p2[(j+1)%p2.size()]);
  }
  return PolyArea(p1);
}

main() {
  int N;
  while (cin >> N) {
    Point v1, v2;
    vector<Point> p1(N);
    for (int i = 0; i < N; i++) cin >> p1[i].x >> p1[i].y;
    cin >> v1.x >> v1.y;
    cin >> N;
    vector<Point> p2(N);
    for (int i = 0; i < N; i++) cin >> p2[i].x >> p2[i].y;
    cin >> v2.x >> v2.y;
    v1 = v1-v2;

    vector<double> vt;
    for (int swp = 0; swp < 2; swp++) {
      for (int i = 0; i < p1.size(); i++) {
        const Point& p1a = p1[i];
        const Point& p1b = p1[(i+1)%p1.size()];
        double v = crossp(p1b-p1a, v1);
        if (v == 0) continue;
        for (int j = 0; j < p2.size(); j++) {
          Point ip = intersect(p1a, p1b, p2[j], p2[j] + v1);
          if (dotp(p1b-p1a, ip-p1a) < 0) continue;
          if (dotp(p1a-p1b, ip-p1b) < 0) continue;
          double cp = crossp(p1b-p1a, p2[j]-p1a);
          double t = cp/v;
          if (t <= 0) continue;
          vt.push_back(t);
        }
      }
      swap(p1, p2);
      v1 = v1 * -1;
    }
    sort(vt.begin(), vt.end());

    double rett = 1e10, retarea = -1;
    for (int i = 0; i+1 < vt.size(); i++) {
      double lot, hit;
      for (lot = vt[i], hit = vt[i+1]; hit-lot > EPS; ) {
        double t1 = (2*lot + hit)/3;
        double t2 = (2*hit + lot)/3;
        if (calc(p1, p2, v1, t1) > calc(p1, p2, v1, t2)) {
          hit = t2;
        } else {
          lot = t1;
        }
      }
      double ar = calc(p1, p2, v1, vt[i]);
      if (ar > retarea+EPS) {retarea = ar; rett = vt[i];}
      ar = calc(p1, p2, v1, (lot+hit)/2);
      if (ar > retarea+EPS) {retarea = ar; rett = (lot+hit)/2;}
    }

    if (vt.size() == 0) {
      printf("never\n");
    } else if (vt.size() == 1) {
      printf("%.6lf\n", vt[0]);
    } else {
      printf("%.6lf\n", rett);
    }
  }
}
