#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
  double x, y;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  Point operator-(const Point& p) const {return Point(x - p.x, y - p.y);}
  Point operator+(const Point& p) const {return Point(x + p.x, y + p.y);}
  Point operator*(const double& c) const {return Point(x * c, y * c);}
  Point operator/(const double& c) const {return Point(x / c, y / c);}
  bool operator<(const Point& p) const {return x == p.x ? y < p.y : x < p.x;}
  bool operator==(const Point& p) const {return x == p.x && y == p.y;}
  double len() {return hypot(x, y);}
};

double dotp(const Point& a, const Point& b) {return a.x*b.x + a.y*b.y;}
double crossp(const Point& a, const Point& b) {return a.x*b.y - a.y*b.x;}

void LineCircleIntersection(const Point& a1, const Point& a2, double R,
    Point& i1, Point& i2) {
  i1 = i2 = Point(1e18, 1e18);
  double d = fabs(crossp(a1, a2-a1) / (a2-a1).len());
  if (d >= R) return;
  double x1 = dotp(a1, a2-a1) / (a2-a1).len();
  double x2 = dotp(a2, a2-a1) / (a2-a1).len();
  double y1 = -sqrt(R*R-d*d);
  double y2 = sqrt(R*R-d*d);
  if (y1 >= x1 && y1 <= x2 || y1 >= x2 && y1 <= x1) {
    i1 = a1 + (a2-a1)*(y1-x1)/(x2-x1);
  }
  if (y2 >= x1 && y2 <= x2 || y2 >= x2 && y2 <= x1) {
    i2 = a1 + (a2-a1)*(y2-x1)/(x2-x1);
    if (i1.x == 1e18) swap(i1, i2);
  }
}

double add_seg(const Point& a1, const Point& a2) {
  //cout << "line   " << a1.x << ',' << a1.y << " to "
  //                  << a2.x << ',' << a2.y << endl;
  return crossp(a1, a2) / 2;
}

double add_sector(const Point& a1, const Point& a2, double R) {
  //cout << "sector " << a1.x << ',' << a1.y << " to "
  //                  << a2.x << ',' << a2.y << endl;
  return R*R * atan2(crossp(a1, a2), dotp(a1, a2)) / 2;
}

main() {
  int N, R;
  while (cin >> N >> R) {
    vector<Point> P(N);
    for (int i = 0; i < N; i++) cin >> P[i].x >> P[i].y;
    double ret = 0.0;

    int sp;
    for (sp = 0; sp < N; sp++) if (P[sp].len() < R) break;

    double ix, iy;
    Point lp(1e18, 1e18);
    for (int i = 0; i < N; i++) {
      Point p1 = P[(sp+i)%N], p2 = P[(sp+i+1)%N];
      Point i1, i2;
      LineCircleIntersection(p1, p2, R, i1, i2);
      if (p1.len() < R) {
        if (p2.len() < R) {
          assert(i1.x == 1e18);
          assert(i2.x == 1e18);
          ret += add_seg(p1, p2);
        } else {
          assert(i1.x != 1e18);
          assert(i2.x == 1e18);
          ret += add_seg(p1, i1);
          lp = i1;
        }
      } else {
        if (p2.len() < R) {
          assert(lp.x != 1e18);
          assert(i1.x != 1e18);
          assert(i2.x == 1e18);
          ret += add_sector(lp, i1, R);
          ret += add_seg(i1, p2);
          lp = Point(1e18, 1e18);
        } else {
          if (i1.x != 1e18) {
            assert(i2.x != 1e18);
            ret += add_seg(i1, i2);
            ret += add_sector(i2, i1, R);
          }
        }
      }
    }
    
    printf("%.3lf\n", ret);
  }
}
