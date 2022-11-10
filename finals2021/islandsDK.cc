#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

const long double PI = 2*acosl(0);

struct Point {
  long double x, y;
  Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
  Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }
  Point operator*(long double c) const { return {x*c, y*c}; }
  Point operator/(long double c) const { return {x/c, y/c}; }
  long double len() const { return hypot(x, y); }
};

inline long double DotProd(const Point& a, const Point& b) {
  return a.x*b.x + a.y*b.y;
}

inline long double CrossProd(const Point& a, const Point& b) {
  return a.x*b.y - a.y*b.x;
}

bool LineSegIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2) {
  long double cp1 = CrossProd(b2-b1, a1-b1);
  long double cp2 = CrossProd(b2-b1, a2-b1);
  if (cp1 > 0 && cp2 > 0) return false;
  if (cp1 < 0 && cp2 < 0) return false;
  cp1 = CrossProd(a2-a1, b1-a1);
  cp2 = CrossProd(a2-a1, b2-a1);
  if (cp1 > 0 && cp2 > 0) return false;
  if (cp1 < 0 && cp2 < 0) return false;
  return true;
}

int main() {
  int N, M, NI;
  while (cin >> N >> M) {
    vector<vector<Point>> I(N);
    for (auto& island : I) {
      cin >> NI;
      island.resize(NI);
      for (int i = 0; i < NI; i++) cin >> island[i].x >> island[i].y;
    }
    vector<Point> F1(M), F2(M);
    vector<long double> FZ1(M), FZ2(M);
    for (int i = 0; i < M; i++) cin >> F1[i].x >> F1[i].y >> FZ1[i] >> F2[i].x >> F2[i].y >> FZ2[i];

    long double lo = 0.0, hi = PI/2;
    for (int rep = 0; rep < 64; rep++) {
      long double th = (hi+lo)/2;
      vector<bool> seen(N);
      for (int f = 0; f < M; f++) {
        vector<Point> poly;
        Point ortho{F1[f].y-F2[f].y, F2[f].x-F1[f].x};
        ortho = ortho / ortho.len();
        poly.push_back(F1[f] - ortho * (FZ1[f]*tan(th)));
        poly.push_back(F2[f] - ortho * (FZ2[f]*tan(th)));
        poly.push_back(F2[f] + ortho * (FZ2[f]*tan(th)));
        poly.push_back(F1[f] + ortho * (FZ1[f]*tan(th)));
        long double mxx = 1e7;
        for (auto [x, _] : poly) mxx = max(mxx, x);
        for (int i = 0; i < I.size(); i++) if (!seen[i]) {
          bool fail = false;
          for (auto const& p : I[i]) {
            int cnt = 0;
            for (int i = 0; i < poly.size(); i++) {
              Point& a = poly[i];
              Point& b = poly[(i+1)%poly.size()];
              cnt += LineSegIntersection(a, b, p, Point{mxx+1337, p.y+7331});
            }
            if (cnt%2 == 0) { fail = true; break; }
          }
          if (!fail) seen[i] = true;
        }
      }
      if (seen == vector<bool>(N, true)) hi = th; else lo = th;
    }

    if (hi == PI/2) {
      cout << "impossible" << endl;
    } else {
      cout << fixed << setprecision(9) << (hi+lo)/2 * 180/PI << endl;
    }
  }
}
