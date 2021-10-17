#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

#define EPS 1e-6

struct Point {
  vector<double> c;
  Point(int d) : c(d) {}
  Point(const vector<double>& c) : c(c) {}
  Point operator-(const Point& p) const {
    Point ret(c);
    for (int i = 0; i < (int)c.size(); i++) ret.c[i] -= p.c[i];
    return ret;
  }
  Point operator*(double x) const {
    Point ret(c);
    for (int i = 0; i < (int)c.size(); i++) ret.c[i] *= x;
    return ret;
  }
  double len() const {
    double ret = 0.0;
    for (auto x : c) ret += x*x;
    return sqrt(ret);
  }
  double dot(const Point& p) const {
    double ret = 0.0;
    for (int i = 0; i < (int)c.size(); i++) ret += c[i] * p.c[i];
    return ret;
  }
};

int main() {
  int D, N;
  while (cin >> D >> N) {
    Point sc(D);
    double sr;
    for (int i = 0; i < D; i++) cin >> sc.c[i];
    cin >> sr;
    vector<Point> X{sc};
    vector<double> E{sr};

    vector<Point> comp;  // Vectors spanning complement of hyperplane.
    for (int n = 1; n < N; n++) {
      Point sc2(D);
      double sr2;
      for (int i = 0; i < D; i++) cin >> sc2.c[i];
      cin >> sr2;
      X.push_back(sc2);
      E.push_back(sr2);

      // Flatten hypersphere onto hyperplane.
      Point cv = sc - sc2;
      for (auto const& v : comp) cv = cv - v * cv.dot(v);
      double cd = ((sc - sc2) - cv).len();
      //assert(sr2*sr2 - cd*cd > -EPS);  // TODO: Why is this assertion blowing?
      sr2 = sqrt(max(0.0, sr2*sr2 - cd*cd));

      // Add vector between hypersphere centers to complement of hyperplane.
      double dist = cv.len();
      if (dist < EPS) {
        //assert(abs(sr-sr2) < EPS);  // TODO: Why is this assertion blowing?
        continue;  // Spheres overlap.
      }
      cv = cv * (1/dist);
      comp.push_back(cv);

      // Intersect hyperspheres.
      assert(abs(sr-sr2)-EPS < dist && dist < sr+sr2+EPS);
      double x = (dist*dist - sr2*sr2 + sr*sr) / dist / 2;
      double y = sqrt(max(0.0, sr*sr - x*x));
      sc = sc - cv * x;
      sr = y;
    }

    Point ret(D);
    for (int i = 0; i < D; i++) ret.c[i] = rand()%1000000 + 1000000;
    for (auto const& v : comp) ret = ret - v * ret.dot(v);
    ret = ret.len() < EPS ? sc : sc - ret * (sr / ret.len());
    for (int i = 0; i < D; i++) {
      if (i) putchar(' ');
      printf("%.17lf", ret.c[i]);
    }
    putchar('\n');
    for (int i = 0; i < N; i++) {
      double err1 = abs((X[i]-ret).len()-E[i]);
      double err2 = abs((X[i]-ret).len()/E[i]-1);
      double err = min(err1, err2);
      if (err > 1e-5)
        fprintf(stderr, "UH-OH! %.9lf %.9lf %.9lf\n", err, (X[i]-ret).len(), E[i]);
    }
  }
}
