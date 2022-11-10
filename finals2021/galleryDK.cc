#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

#define EPS 1e-9

struct Point {
  long double x, y;
  Point(long double x = 0.0, long double y = 0.0) : x(x), y(y) {}
  Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }
  Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
  Point operator*(long double c) const { return {x*c, y*c}; }
  Point operator/(long double c) const { return {x/c, y/c}; }
  long double Len() const { return hypot(x, y); }
};

inline long double CrossProd(const Point& a, const Point& b) {
  return a.x*b.y - a.y*b.x;
}

inline long double DotProd(const Point& a, const Point& b) {
  return a.x*b.x + a.y*b.y;
}

long double RayIntersect(const Point& a, const Point& b, const Point& c, const Point& d, int* sides = NULL) {
  long double cp1 = CrossProd(c-a, b-a), cp2 = CrossProd(d-a, b-a);
  long double dp1 = DotProd(c-a, b-a), dp2 = DotProd(d-a, b-a);
  if (sides) *sides = (cp1 < -EPS || cp2 < -EPS) + 2 * (cp1 > EPS || cp2 > EPS);
  if (cp1 < -EPS && cp2 < -EPS || cp1 > EPS && cp2 > EPS) return -1.0;
  return (abs(cp1) < EPS && abs(cp2) < EPS) ? min(dp1, dp2) : (dp1*cp2-dp2*cp1)/(cp2-cp1);
}

bool PointOnLine(const Point& a, const Point& b, const Point& p) {
  long double ln = (b-a).Len(), cp = CrossProd(b-a, p-a), dp = DotProd(b-a, p-a);
  return abs(cp/ln) < EPS && dp/ln > -EPS && dp/ln < ln+EPS;
}

int N;
vector<Point> p;

int main() {
  while (cin >> N) {
    p.resize(N+2);
    for (int i = 0; i < N+2; i++) cin >> p[i].x >> p[i].y;

    for (int i = 0; i < N+1; i++) {
      Point a = p[N+1], b = p[i];
      if ((b-a).Len() < EPS) { p.push_back(b); continue; }
      b = (b-a)/(b-a).Len() + a;
      vector<pair<long double, int>> inter;
      for (int j = 0; j < N; j++) {
        int sides = 0;
        long double rd = RayIntersect(a, b, p[j], p[(j+1)%N], &sides);
        if (rd < 0) continue;
        inter.push_back({rd, sides});
      }
      sort(inter.begin(), inter.end());
      long double maxd = 0.0;
      for (int j = 0, sides = 0; sides != 3; j++) {
        maxd = inter[j].first;
        sides |= inter[j].second;
      }
      p.push_back((b-a)*maxd + a);
      for (int j = 0; j <= N; j++) {
        long double rd = RayIntersect(a, b, p[j], p[j]+Point(b.y-a.y, a.x-b.x)*1.1e3);
        if (rd < 0) rd = RayIntersect(a, b, p[j], p[j]+Point(a.y-b.y, b.x-a.x)*1.1e3);
        if (rd > EPS && rd < maxd-EPS) p.push_back((b-a)*rd + a);
      }
    }

    vector<long double> dist(p.size(), 1e10);
    priority_queue<pair<long double, int>> q;
    q.push({0.0, N});
    for (;;) {
      int i = q.top().second;
      if (i > N) break;
      long double d = -q.top().first;
      q.pop();
      if (d >= dist[i]) continue;
      dist[i] = d;
      for (int j = 0; j < p.size(); j++) {
        Point a = p[i], b = p[j];
        long double ln = (b-a).Len();
        int ni = 0;
        if (ln < EPS) goto pass;
        if (i < N && PointOnLine(p[i], p[(i+1  )%N], p[j])) goto pass;
        if (i < N && PointOnLine(p[i], p[(i+N-1)%N], p[j])) goto pass;
        b = (b-a)/ln + a;
        for (int k = 0; k < N; k++) {
          long double rd = RayIntersect(a, b, p[k], p[(k+1)%N]);
          if (rd > EPS && rd < ln-EPS) goto fail;
        }
        a = (p[i]+p[j])/2;
        b = a+Point(cos(42), sin(42));
        for (int k = 0; k < N; k++) {
          long double rd = RayIntersect(a, b, p[k], p[(k+1)%N]);
          ni += (rd > EPS);
        }
        if (ni%2 == 0) goto fail;
pass:   q.push({-d-ln, j});
fail:;
      }
    }
    printf("%.12Lf\n", -q.top().first);
  }
}
