#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#define EPS 1e-6

struct Point {
  double x, y;
  Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}
  Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }
  Point& operator+=(const Point& p) { x += p.x; y += p.y; return *this; }
  Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
  Point& operator-=(const Point& p) { x -= p.x; y -= p.y; return *this; }
  Point operator-() const { return {-x, -y}; }
  Point operator*(double c) const { return {x*c, y*c}; }
  Point& operator*=(double c) { x *= c; y *= c; return *this; }
  friend Point operator*(double c, const Point& p) { return {c*p.x, c*p.y}; }
  Point operator/(double c) const { return {x/c, y/c}; }
  Point& operator/=(double c) { x /= c; y /= c; return *this; }
  double LenSqr() const { return x*x + y*y; }
  double Len() const { return hypot(x, y); }
  friend ostream& operator<<(ostream& out, const Point& p) { out << '(' << p.x << ',' << p.y << ')'; return out; }
};

// NOTE: Returns negative area if points are clockwise.
double PolygonArea(const vector<Point>& poly) {
  double ret = 0.0;
  for (int i = 0; i < poly.size(); i++) {
    int j = (i+1)%poly.size();
    ret += poly[i].x * poly[j].y - poly[i].y * poly[j].x;
  }
  return ret/2.0;
}

// Positive if b points counterclockwise of a.
inline double CrossProd(const Point& a, const Point& b) {
  return a.x*b.y - a.y*b.x;
}

int main() {
  double X, Y, N;
  while (cin >> X >> Y >> N) {
    vector<Point> D(N);
    for (auto& [x, y] : D) cin >> x >> y;
    vector<int> perm(N), pos(N);
    for (auto& n : perm) cin >> n;
    for (int i = 0; i < N; i++) pos[perm[i]-1] = i;

    vector<Point> poly{{0, 0}, {X, 0}, {X, Y}, {0, Y}};
    bool outside = false;
    for (int i = 0; i < N; i++)
    for (int j = i+1; j < N; j++) {
      Point p1 = D[i], p2 = D[j];
      if (pos[i] < pos[j]) swap(p1, p2);
      p2 = p1 + (p2-p1) / (p2-p1).Len();

      bool hull = true;
      for (auto const& p : D) {
        if (CrossProd(p2-p1, p-p1) > EPS) hull = false;
      }
      if (hull) outside = true;

      bool over = true, under = true;
      for (auto const& p : poly) {
        double cp = CrossProd(p2-p1, p-p1);
        if (cp > EPS) under = false;
        if (cp < -EPS) over = false;
      }
      if (under) goto fail;
      if (over) continue;

      int i1 = -1, i2 = -1;
      for (int k = 0; k < poly.size(); k++) {
        const Point& a = poly[k];
        const Point& b = poly[(k+1)%poly.size()];
        if (CrossProd(p2-p1, a-p1) >  EPS && CrossProd(p2-p1, b-p1) <= EPS) i1 = k;
        if (CrossProd(p2-p1, a-p1) <= EPS && CrossProd(p2-p1, b-p1) >  EPS) i2 = k;
      }
      if (i1 == -1 || i2 == -1) continue;
      vector<Point> poly2;
      {
        const Point& a = poly[i2];
        const Point& b = poly[(i2+1)%poly.size()];
        double acp = CrossProd(p2-p1, a-p1);
        double bcp = CrossProd(p2-p1, b-p1);
        poly2.push_back((a*bcp - b*acp) / (bcp - acp));
      }
      if (i1 > i2) {
        for (int k = i2+1; k <= i1; k++) poly2.push_back(poly[k]);
      } else {
        for (int k = i2+1; k < poly.size(); k++) poly2.push_back(poly[k]);
        for (int k = 0; k <= i1; k++) poly2.push_back(poly[k]);
      }
      {
        const Point& a = poly[i1];
        const Point& b = poly[(i1+1)%poly.size()];
        double acp = CrossProd(p2-p1, a-p1);
        double bcp = CrossProd(p2-p1, b-p1);
        poly2.push_back((a*bcp - b*acp) / (bcp - acp));
      }
      poly.swap(poly2);
//cout << outside << ' ';
//cout << D[i] << '-' << D[j] << ": ";
//for (auto const& p : poly) cout << ' ' << p;
//cout << endl;
    }
    if (!outside && N > 1) goto fail;

    printf("%.6lf\n", PolygonArea(poly));
    continue;

fail:
    cout << "0" << endl;
  }
}
