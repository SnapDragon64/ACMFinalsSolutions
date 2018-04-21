#include <cmath>
#include <cstring>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

#define EPS 1e-9
#define G 9.80665

bool intersect(double vd, double vh, double dx, double dy, double x, double y1, double y2, double bh) {
  if (dx < 0.0) { dx = -dx; x = -x; }
  if (x < 0.0 || x > dx) return false;
  if (dx*y1 > x*dy) return false;  // exact
  if (dx*y2 < x*dy) return false;  // exact
  // d(t) = vd * t, h(t) = vh * t - 1/2 * G * t^2
  double d = hypot(x, x*dy/dx);
  double t = d / vd;
  double h = vh * t - G/2.0 * t*t;
  //if (h < bh+EPS) cout << "  fail " << d << " " << t << " " << h << endl;
  return h < bh+EPS;
}

int main() {
  double w, v;
  int dx, dy, lx, ly;
  while (cin >> dx >> dy >> w >> v >> lx >> ly) {
    vector<vector<int>> g(dy+1, vector<int>(dx+1));
    vector<vector<int>> dist(dy+1, vector<int>(dx+1, 1000000000));
    for (int y = 1; y <= dy; y++)
    for (int x = 1; x <= dx; x++) {
      cin >> g[y][x];
    }

    int curd = 0;
    vector<pair<int, int>> q{{lx, ly}};
    dist[ly][lx] = 0;
    while (!q.empty()) {
      curd++;
      vector<pair<int, int>> q2;
      for (auto c : q) {
        int x, y;
        tie(x, y) = c;
        for (int x2 = 1; x2 <= dx; x2++)
        for (int y2 = 1; y2 <= dy; y2++) if (dist[y2][x2] > curd) {
          double d = w * hypot(x2-x, y2-y);
          double h = g[y2][x2]-g[y][x];
          double a = h*h + d*d;
          double b = -2*h*h*v*v - G*d*d*h - d*d*v*v;
          double c = h*h*v*v*v*v + G*d*d*h*v*v + G*G*d*d*d*d/4;
          double disc = b*b-4*a*c;
          if (disc < -EPS) continue;
          double vhsqr = (-b+sqrt(disc))/2/a;  // Two vh will work; choose higher one.
          if (vhsqr < EPS || vhsqr > v*v-EPS) continue;
          double vd = sqrt(v*v-vhsqr), vh = sqrt(vhsqr);
          //cout << '(' << x << ',' << y << ") -> (" << x2 << ',' << y2 << ") d=" << d << " h=" << h;
          //cout << " a=" << a << " b=" << b << " c=" << c << " vd=" << vd << " vh=" << vh;
          //cout << " h2=" << vh*(d/vd) - G/2*(d/vd)*(d/vd) << endl;

          bool fail = false;
          for (int x3 = min(x, x2); !fail && x3 <= max(x, x2); x3++)
          for (int y3 = min(y, y2); !fail && y3 <= max(y, y2); y3++) {
            fail |= intersect(vd/w, vh, x2-x, y2-y, x3+0.5-x, y3-0.5-y, y3+0.5-y, g[y3][x3]-g[y][x]);
            fail |= intersect(vd/w, vh, x2-x, y2-y, x3-0.5-x, y3-0.5-y, y3+0.5-y, g[y3][x3]-g[y][x]);
            fail |= intersect(vd/w, vh, y2-y, x2-x, y3+0.5-y, x3-0.5-x, x3+0.5-x, g[y3][x3]-g[y][x]);
            fail |= intersect(vd/w, vh, y2-y, x2-x, y3-0.5-y, x3-0.5-x, x3+0.5-x, g[y3][x3]-g[y][x]);
          }
          if (fail) continue;

          q2.push_back({x2, y2});
          dist[y2][x2] = curd;
        }
      }
      q.swap(q2);
    }

    for (int y = 1; y <= dy; y++) {
      for (int x = 1; x <= dx; x++) {
        if (x > 1) cout << ' ';
        if (dist[y][x] > dx*dy) cout << 'X'; else cout << dist[y][x];
      }
      cout << endl;
    }
  }
}
