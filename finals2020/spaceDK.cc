#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

// Returned values (x,y,g) satisfy x*a+y*b = g.
// |x|+|y| is minimized, breaking ties with |x| <= |y|.
// If g>0, all other solutions are of the form (x+n*(b/g))*a + (y-n*(a/g))*b = g.
template<typename T> constexpr tuple<T,T,T> ExtendedGcd(T a, T b) {
	if( b == 0 ) {
    if (a == 0) return {0, 0, 0};
    if (a < 0) return {-1, 0, -a};
    return {1, 0, a};
	}
  T x, y, g;
  tie(x, y, g) = ExtendedGcd(b, a%b);
  return {y, x-y*(a/b), g};
}

struct Vec {
  int x, y, z;
  Vec operator-() const { return Vec{-x, -y, -z}; }
  Vec operator+(const Vec& v) const { return Vec{x+v.x, y+v.y, z+v.z}; }
  Vec &operator+=(const Vec& v) { x += v.x; y += v.y; z += v.z; return *this; }
  Vec &operator-=(const Vec& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
  Vec operator*(int c) const { return Vec{x*c, y*c, z*c}; }
  bool operator==(const Vec& v) const { return x == v.x && y == v.y && z == v.z; }
};

struct Leg {
  Vec s, f, d;
  int t;
  Leg operator-() const { return Leg{s+d*(t-1), f, -d, t}; }
  bool operator==(const Leg& l) const { return s == l.s && f == l.f && d == l.d && t == l.t; }
};

Vec readDir() {
  Vec ret{0, 0, 0};
  string s;
  cin >> s;
  (s[0] == 'x' ? ret.x : s[0] == 'y' ? ret.y : ret.z) = (s[1] == '+' ? 1 : -1);
  return ret;
}

void intersect(const Vec& a1, const Vec& a2, const Vec& b1, const Vec& b2, Vec* o1, Vec* o2) {
  o1->x = max(min(a1.x, a2.x), min(b1.x, b2.x));
  o1->y = max(min(a1.y, a2.y), min(b1.y, b2.y));
  o1->z = max(min(a1.z, a2.z), min(b1.z, b2.z));
  o2->x = min(max(a1.x, a2.x), max(b1.x, b2.x));
  o2->y = min(max(a1.y, a2.y), max(b1.y, b2.y));
  o2->z = min(max(a1.z, a2.z), max(b1.z, b2.z));
}

int main() {
  int N, K;
  while (cin >> N >> K) {
    vector<Vec> SA(N), SB(N);
    for (int i = 0; i < N; i++) {
      cin >> SA[i].x >> SA[i].y >> SA[i].z >> SB[i].x >> SB[i].y >> SB[i].z;
      SB[i] -= Vec{1, 1, 1};
    }

    vector<vector<Leg>> r(K);
    vector<int64_t> rc(K);
    for (int i = 0; i < K; i++) {
      Leg leg;
      cin >> leg.s.x >> leg.s.y >> leg.s.z;
      leg.f = readDir();
      leg.d = readDir();

      for (;;) {
        leg.t = 0;
        bool change;
        do {
          change = false;
          for (int j = 0; j < N; j++) {
            Vec v = leg.s + leg.d*leg.t;
            if (v.x >= SA[j].x && v.y >= SA[j].y && v.z >= SA[j].z &&
                v.x <= SB[j].x && v.y <= SB[j].y && v.z <= SB[j].z) {
              Vec a, b;
              intersect(v, v + leg.d*1e9, SA[j], SB[j], &a, &b);
              leg.t += (b.x-a.x)+(b.y-a.y)+(b.z-a.z)+1;
              change = true;
            }
          }
        } while(change);
        bool collide = false;
        for (int j = 0; j < N; j++) {
          Vec v = leg.s + leg.f, a, b;
          intersect(v, v + leg.d*leg.t, SA[j], SB[j], &a, &b);
          if (b.x >= a.x && b.y >= a.y && b.z >= a.z) {
            leg.t = min(abs(v.x-a.x)+abs(v.y-a.y)+abs(v.z-a.z), abs(v.x-b.x)+abs(v.y-b.y)+abs(v.z-b.z));
            collide = true;
          }
        }
        assert(leg.t > 0);

        if (r[i].size() >= 2 && leg == r[i][1]) break;
        if (r[i].size()) rc[i] += leg.t;
//cout << "r" << i << " leg" << r[i].size() << ": ";
//cout << leg.s.x << ',' << leg.s.y << ',' << leg.s.z << ' ' << leg.t << ' ';
//cout << leg.f.x << ',' << leg.f.y << ',' << leg.f.z << ' ';
//cout << leg.d.x << ',' << leg.d.y << ',' << leg.d.z << endl;
        r[i].push_back(leg);

        if (collide) {
          leg.s = leg.s + leg.f + leg.d*leg.t;
          swap(leg.f, leg.d);
          leg.f = -leg.f;
        } else {
          leg.s = leg.s + leg.d*(leg.t-1);
          swap(leg.f, leg.d);
          leg.d = -leg.d;
        }
      }
//cout << "rc" << i << ": " << rc[i] << endl;
    }

    int64_t ret = 1e18;
    for (int i = 0; i < K; i++)
    for (int j = i+1; j < K; j++) {
      if (r[i][0].s == r[j][0].s && r[i][0].f == r[j][0].f) ret = 0;

      bool opp = false;
      for (int ji = 1, jt = r[j][0].t; ji < r[j].size(); jt += r[j][ji].t, ji++) if (r[j][ji] == -r[i][1]) {
        ret = min(ret, int64_t((jt+r[i][1].t+r[i][0].t-1)/2) % (rc[i]/2));
        opp = true;
      }
      if (opp) continue;

      for (int ii = 1, it = r[i][0].t; ii < r[i].size(); it += r[i][ii].t, ii++)
      for (int ji = 1, jt = r[j][0].t; ji < r[j].size(); jt += r[j][ji].t, ji++) if (r[i][ii].f == r[j][ji].f) {
        Vec a, b;
        intersect(r[i][ii].s, r[i][ii].s+r[i][ii].d*(r[i][ii].t-1),
                  r[j][ji].s, r[j][ji].s+r[j][ji].d*(r[j][ji].t-1), &a, &b);
        if (a == b) {
          int64_t t1 = it + abs(r[i][ii].s.x-a.x) + abs(r[i][ii].s.y-a.y) + abs(r[i][ii].s.z-a.z);
          int64_t t2 = jt + abs(r[j][ji].s.x-a.x) + abs(r[j][ji].s.y-a.y) + abs(r[j][ji].s.z-a.z);
          int64_t x, y, g;
          tie(x, y, g) = ExtendedGcd(rc[i], rc[j]);
          if (t1%g == t2%g) {
//cout << t1 << ' ' << t2 << ' ' << x << ' ' << y << ' ' << g << ' ' << rc[i] << ' ' << rc[j] << endl;
            int64_t c = rc[i]*rc[j]/g, t = (t1*y)%rc[i]*rc[j]/g + (t2*x)%rc[j]*rc[i]/g;
            ret = min(ret, (t%c+c)%c);
          }
        }
      }
    }

    if (ret == 1e18) cout << "ok" << endl; else cout << ret << endl;
  }
}
