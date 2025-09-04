#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace std;

struct Link {
  int ei1, ei2;
  double fz, fc;
  bool operator<(const Link& l) const { return false; }
  Link operator+(const Link& l) const {
    assert(ei2 == l.ei1);
    return Link{ei1, l.ei2, fz+l.fz, fc+l.fc};
  }
  Link rev() const { return Link{ei2, ei1, fz, fc}; }
};

struct Edge {
  int a, b, border;
  vector<vector<Link>> skip{{}, {}};
};

int main() {
  int T, X, Y, N, M, A, B, C;
  for (cin >> T; T--;) {
    cin >> X >> Y >> N >> M;
    vector<int64_t> vx(N), vy(N), vz(N);
    for (int i = 0; i < N; i++) cin >> vx[i] >> vy[i] >> vz[i];

    vector<Edge> e;
    map<pair<int,int>, int> ei;
    auto edge_idx = [&](int a, int b) {
      if (ei.count({a, b})) return ei[{a, b}];
      int ret = ei[{a, b}] = e.size();
      e.push_back(Edge{a: a, b: b, border: vx[a] == 0 && vx[b] == 0 ? 1 : vx[a] == X && vx[b] == X ? 2 : 0});
      return ret;
    };

    vector<tuple<int64_t,bool,int,Link>> events;
    for (int i = 0; i < M; i++) {
      cin >> A >> B >> C; A--; B--; C--;
      while (vz[A] > vz[B] || vz[A] > vz[C]) { swap(A, B); swap(B, C); }
      bool flip = false;
      if (vz[B] > vz[C]) { swap(B, C); flip = true; }
      double mx = vx[A] + (vx[C]-vx[A])*(vz[B]-vz[A])/double(vz[C]-vz[A]);
      double my = vy[A] + (vy[C]-vy[A])*(vz[B]-vz[A])/double(vz[C]-vz[A]);
      double ml = hypot(mx-vx[B], my-vy[B]);

      for (int j = 0; j < 2; j++) {
        int lo = j?B:A, hi = j?C:B, zero=j?C:A;
        double fz = ml / (vz[B]-vz[zero]), fc = -fz * vz[zero];
        Link link{edge_idx(lo, hi), edge_idx(A, C), fz, fc};
        if (flip) swap(link.ei1, link.ei2);
        events.push_back({vz[lo], true , lo, link});
        events.push_back({vz[hi], false, hi, link});
      }
    }
    sort(events.begin(), events.end());

    double ret = 1e18;
    int maxskip = 0;
    for (int ei = 0; ei < e.size(); ei++) {
      do {
        e[ei].skip[0].push_back(Link{ei, -1, 0.0, 0.0});
        e[ei].skip[1].push_back(Link{ei, -1, 0.0, 0.0});
      } while (rand()%2);
    }
    function<Link(int,int,int,int)> follow = [&](int ei, int dir, int h, int rep) {
      auto const& s = e[ei].skip[dir];
      maxskip = max<int>(maxskip, s.size());
      if (s[h].ei2 == rep) return Link{ei, ei, 1e50, 1e50};  // cycle
      while (h+1 < s.size() && s[h+1].ei2 != -1) { h++; rep = ei; }
      while (h > 0 && s[h].ei2 == -1) h--;
      if (s[h].ei2 == -1) return Link{ei, ei, 0.0, 0.0};
      return s[h] + follow(s[h].ei2, dir, h, rep);
    };

    for (int i = 0; i < events.size(); i++) {
      auto [z, add, zv, link] = events[i];

      if (i > 0 && z != get<0>(events[i-1])) {
        vector<double> border(3, 1e50);
        if (vx[zv] == 0) border[1] = 0.0;
        if (vx[zv] == X) border[2] = 0.0;
        for (int j = i; j < events.size(); j++) {
          auto [z2, add2, zv2, link2] = events[j];
          if (z2 != z || add2) break;
          for (int dir = 0; dir < 2; dir++) {
            link2 = link2.rev();
            if (e[link2.ei2].a == zv || e[link2.ei2].b == zv) continue;
            Link link3 = follow(link2.ei1, dir, 0, link2.ei1);
            double& b = border[e[link3.ei2].border];
            b = min(b, link3.fz*z + link3.fc);
          }
        }
        ret = min(ret, border[1]+border[2]);
      }

      maxskip = 0;
      follow(link.ei2, 1, 0, link.ei2);
      if (add) {
        for (int h = 0; h < maxskip; h++) {
          while (link.ei1 != -1 && e[link.ei1].skip[1].size() <= h) link = e[link.ei1].skip[0][h-1].rev() + link;
          while (link.ei2 != -1 && e[link.ei2].skip[1].size() <= h) link = link + e[link.ei2].skip[1][h-1];
          if (link.ei1 == -1 || link.ei2 == -1) break;
          e[link.ei1].skip[1][h] = link;
          e[link.ei2].skip[0][h] = link.rev();
        }
      } else {
        for (int dir = 0; dir < 2; dir++) {
          int ei = dir ? link.ei2 : link.ei1;
          for (int h = 0; h < maxskip; h++) {
            while (ei != -1 && e[ei].skip[dir].size() <= h) ei = e[ei].skip[dir][h-1].ei2;
            if (ei == -1) break;
            e[ei].skip[!dir][h] = Link{ei, -1, 0.0, 0.0};
          }
        }
      }
    }

    if (ret == 1e18) cout << "impossible" << endl; else cout << fixed << setprecision(10) << ret << endl;
  }
}
