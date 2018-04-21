// NOTE: This is a solution to the original version of Panda Preserve, which
// required an O(n log n) solution.  The O(n log n) algorithm is absolutely
// horrible, and my Voronoi code is horrible, and nobody should ever solve
// the problem this way. :)

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

#define EPS 1e-6
#define LARGE 1e20

struct Point {
  double x, y;
  Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}
  Point operator-(const Point& p) const { return Point(x-p.x, y-p.y); }
  Point operator+(const Point& p) const { return Point(x+p.x, y+p.y); }
  Point operator*(double c) const { return Point(x*c, y*c); }
  Point operator/(double c) const { return Point(x/c, y/c); }
  Point operator-() const { return Point(-x, -y); }
  double len() const { return hypot(x, y); }
  Point rotate(const double th) const {
    return Point(x*cos(th) - y*sin(th), x*sin(th) + y*cos(th));
  }
  bool operator<(const Point& p) const {
    return x < p.x-EPS || (x < p.x+EPS && y < p.y-EPS);
  }

  friend ostream& operator<<(ostream& out, const Point& p) {
    Point p2 = p.rotate(-0.0666);
    if (fabs(p2.x) < EPS) p2.x = 0;
    if (fabs(p2.y) < EPS) p2.y = 0;
    out << p2.x << ',' << p2.y;
    return out;
  }
};

double CrossP(const Point& a, const Point& b) {
  return a.x*b.y - a.y*b.x;
}

Point Intersect(const Point& a1, const Point& a2,
                const Point& b1, const Point& b2) {
  double cp1 = CrossP(a2-a1, b1-a1);
  double cp2 = CrossP(a2-a1, b2-a1);
  if (fabs(cp2-cp1) < EPS) return Point(LARGE, LARGE);
  return (b1*cp2 - b2*cp1) / (cp2-cp1);
}

bool DoSegsIntersect(const Point& a1, const Point& a2,
                     const Point& b1, const Point& b2) {
  double cp1 = CrossP(a2-a1, b1-a1);
  double cp2 = CrossP(a2-a1, b2-a1);
  if (fabs(cp1) < EPS && fabs(cp2) < EPS) return false;
  if (cp1 > EPS && cp2 > EPS || cp1 < -EPS && cp2 < -EPS) return false;
  cp1 = CrossP(b2-b1, a1-b1);
  cp2 = CrossP(b2-b1, a2-b1);
  if (cp1 > EPS && cp2 > EPS || cp1 < -EPS && cp2 < -EPS) return false;
  return true;
}

Point Circumcenter(const Point& a, const Point& b, const Point& c) {
  Point a1 = (a+b)/2;
  Point a2 = a1 + Point(b.y-a.y, a.x-b.x);
  Point b1 = (c+b)/2;
  Point b2 = b1 + Point(b.y-c.y, c.x-b.x);
  return Intersect(a1, a2, b1, b2);
}


map<pair<Point, Point>, Point> Voronoi(const vector<Point>& vin) {
  static vector<Point> v = vin;
  sort(v.begin(), v.end());

  struct BeachSeg;
  struct Event {
    double x;
    int vertex;
    mutable set<BeachSeg>::iterator it;
    Event(double x, int vertex) : x(x), vertex(vertex) {};
    bool operator<(const Event& e) const { return x < e.x; }
  };

  struct BeachSeg {
    int ai, bi, ci;
    mutable set<Event>::iterator it;

    double y1(double sweepx) const {
      if (sweepx <= v[bi].x + EPS) return v[bi].y;
      if (ai == -1) return -LARGE;
      if (sweepx <= v[ai].x + EPS) return v[ai].y;
      double a = 0.5 / (v[ai].x-sweepx) - 0.5 / (v[bi].x-sweepx);
      double b = v[ai].y / (sweepx-v[ai].x) - v[bi].y / (sweepx-v[bi].x);
      double c = 0.5 * (v[ai].x*v[ai].x + v[ai].y*v[ai].y - sweepx*sweepx) /
                   (v[ai].x-sweepx) -
                 0.5 * (v[bi].x*v[bi].x + v[bi].y*v[bi].y - sweepx*sweepx) /
                   (v[bi].x-sweepx);
      double y = (-b - sqrt(b*b-4*a*c)) / 2 / a;
      return y;
    }
    double y2(double sweepx) const {
      if (sweepx <= v[bi].x + EPS) return v[bi].y;
      if (ci == -1) return LARGE;
      if (sweepx <= v[ci].x + EPS) return v[ci].y;
      double a = 0.5 / (v[ci].x-sweepx) - 0.5 / (v[bi].x-sweepx);
      double b = v[ci].y / (sweepx-v[ci].x) - v[bi].y / (sweepx-v[bi].x);
      double c = 0.5 * (v[ci].x*v[ci].x + v[ci].y*v[ci].y - sweepx*sweepx) /
                   (v[ci].x-sweepx) -
                 0.5 * (v[bi].x*v[bi].x + v[bi].y*v[bi].y - sweepx*sweepx) /
                   (v[bi].x-sweepx);
      double y = (-b + sqrt(b*b-4*a*c)) / 2 / a;
      return y;
    }

    bool operator<(const BeachSeg& b) const {
      double x = max(v[bi].x, v[b.bi].x);
      if (ai != -1) x = max(x, v[ai].x);
      if (ci != -1) x = max(x, v[ci].x);
      if (b.ai != -1) x = max(x, v[b.ai].x);
      if (b.ci != -1) x = max(x, v[b.ci].x);
      return y2(x) <= b.y1(x) + EPS;
    }

    double eventx() const {
      if (ai == -1 || ci == -1) return LARGE;
      if (CrossP(v[ai]-v[bi], v[ci]-v[bi]) < 0) return LARGE;
      Point c = Circumcenter(v[ai], v[bi], v[ci]);
      return (c.x == LARGE) ? LARGE : c.x + (v[bi]-c).len();
    }
  };

  multiset<Event> event;
  multiset<BeachSeg> beachseg;
  for (int i = 0; i < v.size(); i++) {
    event.insert(Event(v[i].x, i));
  }

  map<pair<Point, Point>, Point> ret;
  while (event.size()) {
    Event e = *event.begin();
    if (e.x >= LARGE) break;
    event.erase(event.begin());

    if (e.vertex != -1) {

      BeachSeg bs;
      bs.ai = -1; bs.bi = e.vertex; bs.ci = -1;
      auto it = beachseg.lower_bound(bs);
      if (it != beachseg.end()) {
        BeachSeg bs2 = *it;
        bs.ai = bs.ci = bs2.bi;
        if (it->it != event.end()) event.erase(it->it);
        beachseg.erase(it);
        int oai = bs2.ai;
        bs2.ai = e.vertex;
        bs2.it = event.insert(Event(bs2.eventx(), -1));
        bs2.it->it = beachseg.insert(bs2);
        bs2.ai = oai;
        bs2.ci = e.vertex;
        bs2.it = event.insert(Event(bs2.eventx(), -1));
        bs2.it->it = beachseg.insert(bs2);
      }
      bs.it = event.insert(Event(bs.eventx(), -1));
      bs.it->it = beachseg.insert(bs);

    } else {

      BeachSeg bs = *e.it;
//cout << " erasing " << v[bs.ai] << ' ' << v[bs.bi] << ' ' << v[bs.ci] << endl;
      Point c = Circumcenter(v[bs.ai], v[bs.bi], v[bs.ci]);
      ret[make_pair(v[bs.ai], v[bs.ci])] = c;
      ret[make_pair(v[bs.bi], v[bs.ai])] = c;
      ret[make_pair(v[bs.ci], v[bs.bi])] = c;
//cout << " adding " << c << "  " << v[bs.ai] << ' ' << v[bs.ci] << endl;
//cout << " adding " << c << "  " << v[bs.bi] << ' ' << v[bs.ai] << endl;
//cout << " adding " << c << "  " << v[bs.ci] << ' ' << v[bs.bi] << endl;

      auto it1 = e.it, it2 = e.it;
      --it1; ++it2;
      BeachSeg bs1 = *it1, bs2 = *it2;
      beachseg.erase(e.it);
      event.erase(it1->it);
      beachseg.erase(it1);
      event.erase(it2->it);
      beachseg.erase(it2);

      bs1.ci = bs2.bi;
      bs1.it = event.insert(Event(bs1.eventx(), -1));
      bs1.it->it = beachseg.insert(bs1);
      bs2.ai = bs1.bi;
      bs2.it = event.insert(Event(bs2.eventx(), -1));
      bs2.it->it = beachseg.insert(bs2);

    }

/*cout << endl << "V event " << e.x << ' ' << e.vertex << endl;
for (auto it : beachseg) {
  if (it.ai == -1) cout << "x  "; else cout << v[it.ai] << "  ";
  cout << v[it.bi] << "  ";
  if (it.ci == -1) cout << "x  "; else cout << v[it.ci] << "  ";
  cout << it.y1(e.x) << ' ' << it.y2(e.x) << ' ' << it.eventx() << endl;
}*/

  }

  return ret;
}


struct Seg {
  int idx;
  Point a, b, vp;
  enum {BOTTOM, VORONOI, TOP} type;

  Seg(int idx, const Point& a, const Point& b, decltype(type) type,
      const Point& vp = Point())
    : idx(idx), a(a), b(b), type(type), vp(vp) {
    if (a.x > b.x) swap(this->a, this->b);
  }

  double interpolate(double x) const {
    if (fabs(b.x-a.x) < EPS) return a.y;
    return a.y + (b.y-a.y) * ((x-a.x) / (b.x-a.x));
  }

  bool operator<(const Seg& s) const {
    double cmpx = max(a.x, s.a.x);
    double y1 = interpolate(cmpx);
    double y2 = s.interpolate(cmpx);
    if (fabs(y1-y2) > EPS) return y1 < y2;
    cmpx = min(b.x, s.b.x);
    y1 = interpolate(cmpx);
    y2 = s.interpolate(cmpx);
    if (fabs(y1-y2) > EPS) return y1 < y2;
    return idx < s.idx;
  }
};

struct Event {
  double x;
  enum {DEL, ADD, INTERSECT} type;
  int segi;

  Event(double x, decltype(type) type, int segi)
    : x(x), type(type), segi(segi) {}

  bool operator<(const Event& e) const {
    return x > e.x+EPS || (x > e.x-EPS && type > e.type);
  }
};

double ret;

void handle_segs(const Seg& s1, const Seg& s2, priority_queue<Event>& q) {
  if ((s1.type == Seg::VORONOI) ^ (s2.type == Seg::VORONOI)) {
    if (DoSegsIntersect(s1.a, s1.b, s2.a, s2.b)) {
      Point ip = Intersect(s1.a, s1.b, s2.a, s2.b);
      ret = max(ret, (((s1.type==Seg::VORONOI) ? s1.vp : s2.vp) - ip).len());
      q.push(Event(ip.x, Event::INTERSECT,
                   (s1.type==Seg::VORONOI) ? s1.idx : s2.idx));
    }
  }
}

void HandleAdjacentSegs(set<Seg>::iterator it, set<Seg>& s,
                        priority_queue<Event>& q) {
  if (it != s.begin()) {
    auto it2 = it;
    --it2;
    handle_segs(*it2, *it, q);
  }
  auto it2 = it;
  ++it2;
  if (it2 != s.end()) {
    handle_segs(*it, *it2, q);
  }
}

int main() {
  int N;
  while (cin >> N) {
    vector<Point> P(N);
    for (int i = 0; i < P.size(); i++) {
      cin >> P[i].x >> P[i].y;
      P[i] = P[i].rotate(0.0666);
    }

    auto vd = Voronoi(P);

    vector<Seg> seg;
    for (int i = 0; i < P.size(); i++) {
      seg.emplace_back(seg.size(), P[i], P[(i+1)%P.size()],
                       (P[i].x < P[(i+1)%P.size()].x) ? Seg::BOTTOM : Seg::TOP);
    }
    for (auto it : vd) {
      Point pa = it.first.first, pb = it.first.second;
      Point a = it.second, b;
      auto it2 = vd.find(make_pair(pb, pa));
      if (it2 == vd.end()) {
        Point perp(pb.y - pa.y, pa.x - pb.x);
        b = a + perp * (1e7 / perp.len());
      } else {
        b = it2->second;
        vd.erase(it2);
      }
      if ((b-a).len() < EPS) continue;
//cout << a << ' ' << b << endl;

      if (CrossP(pb-pa, a-pa) * CrossP(pb-pa, b-pa) > 0) {
        seg.emplace_back(seg.size(), a, b, Seg::VORONOI, pa);
      } else {
        Point mp = (pa+pb)/2;
        seg.emplace_back(seg.size(), a, mp, Seg::VORONOI, pa);
        seg.emplace_back(seg.size(), b, mp, Seg::VORONOI, pa);
      }
    }

    ret = 0.0;
    for (int pass = 0; pass < 3; pass++) {
      priority_queue<Event> q;
      for (int i = 0; i < seg.size(); i++) {
        q.push(Event(seg[i].a.x, Event::ADD, i));
        q.push(Event(seg[i].b.x, Event::DEL, i));
      }

      set<Seg> s;
      while (!q.empty()) {
        Event e = q.top();
        q.pop();
        if (e.x >= LARGE) break;

        if (e.type == Event::ADD) {
          if ((seg[e.segi].b - seg[e.segi].a).len() < EPS) {
            // Single points are a special case.
            auto it = s.lower_bound(seg[e.segi]);
            if (it != s.end() && it->type == Seg::TOP) {
              ret = max(ret, (seg[e.segi].a - seg[e.segi].vp).len());
            }
          } else {
            HandleAdjacentSegs(s.insert(seg[e.segi]).first, s, q);
          }
        } else if (e.type == Event::DEL || e.type == Event::INTERSECT) {
          auto it = s.find(seg[e.segi]);
          if (it != s.end()) {
            HandleAdjacentSegs(it, s, q);
            s.erase(it++);
            if (it != s.end()) HandleAdjacentSegs(it, s, q);
          }
        }

/*cout << endl << "S event " << e.x << " " << e.type;
cout << "  " << seg[e.segi].a << " to " << seg[e.segi].b << endl;
for (auto it : s) {
  cout << it.type << "  " << it.a << " to " << it.b << endl;
}
cout << "  ret = " << ret << endl;*/

      }

      if (pass == 0) {
        // Pass 0: left-right.  Pass 1: right-left.
        for (auto it = seg.begin(); it != seg.end(); ++it) {
          it->a.x = -it->a.x;
          it->b.x = -it->b.x;
          it->vp.x = -it->vp.x;
          swap(it->a, it->b);
        }
      } else if (pass == 1) {
        // Pass 2: Just Voronoi vertices.
        for (int i = seg.size()-1; i >= 0; i--) {
          if (seg[i].type == Seg::VORONOI) {
            seg.push_back(seg[i]);
            seg.back().a = seg.back().b;
            seg[i].b = seg[i].a;
          }
        }
      }
    }

    printf("%.9lf\n", ret);
  }
}
