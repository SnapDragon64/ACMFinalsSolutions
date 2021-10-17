#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

#define EPS 1e-15

struct Distribution {
  vector<double> v;
  int base = 0, mn = 0;

  Distribution() : v{1.0} {}

  Distribution(const Distribution& d) {
    base = d.base + d.mn;
    v.insert(v.begin(), d.v.begin()+d.mn, d.v.end());
  }

  double operator[](int i) const {
    if (i < base+mn) return 0.0;
    if (i-base >= v.size()) return 0.0;
    return v[i-base];
  }

  void Add(double p) {
    v.push_back(p * v.back());
    for (int i = v.size()-2; i > mn; i--) v[i] = (1-p) * v[i] + p * v[i-1];
    v[mn] *= (1-p);
    while (v[mn] < EPS) mn++;
    while (v.back() < EPS) v.pop_back();
  }
};

int X, Y, T, Q;
vector<double> XP, YP;
vector<vector<pair<int, int>>> q;

map<pair<int, int>, vector<pair<int, int>>> node;
const vector<pair<int, int>>& domerge(int s, int e) {
  auto& v = node[{s, e}];
  if (s+1 == e) {
    v = q[s];
  } else {
    auto const& a = domerge(s, (s+e)/2);
    auto const& b = domerge((s+e)/2, e);
    v = a;
    v.insert(v.end(), b.begin(), b.end());
  }
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  return v;
}

void doit(int s, int e, const Distribution& dn) {
  if (s+1 == e) {
    if (s == Q) return;
    Distribution dy;
    for (auto [x, y] : q[s]) dy.Add(XP[x] + YP[y]);
    double tot = 0;
    for (int i = 0; i <= q[s].size(); i++) tot += dy[i] * dn[T-i];
    for (int i = 0; i <= q[s].size(); i++) {
      printf("%.9lf ", dy[i] * dn[T-i] / tot);
    }
    printf("\n");
    return;
  }
  Distribution dna(dn), dnb(dn);
  auto const& a = node[{s, (s+e)/2}];
  auto const& b = node[{(s+e)/2, e}];
  for (int ai = 0, bi = 0; ai < a.size() || bi < b.size(); ) {
    if (bi == b.size() || (ai < a.size() && a[ai] < b[bi])) {
      dnb.Add(XP[a[ai].first] + YP[a[ai].second]);
      ai++;
    } else if (ai == a.size() || b[bi] < a[ai]) {
      dna.Add(XP[b[bi].first] + YP[b[bi].second]);
      bi++;
    } else {
      ai++;
      bi++;
    }
  }
  doit(s, (s+e)/2, dna);
  doit((s+e)/2, e, dnb);
}

int main() {
  while (cin >> X >> Y >> T >> Q) {
    XP.resize(X); YP.resize(Y);
    for (auto& p : XP) cin >> p;
    for (auto& p : YP) cin >> p;
    q.clear();
    q.resize(Q+1);
    for (int i = 0; i < Q; i++) {
      int S;
      cin >> S;
      for (int j = 0; j < S; j++) {
        int x, y;
        cin >> x >> y;
        q[i].emplace_back(x-1, y-1);
      }
    }
    for (int x = 0; x < X; x++) for (int y = 0; y < Y; y++) q[Q].emplace_back(x, y);
    domerge(0, Q+1);
    doit(0, Q+1, Distribution());
  }
}
