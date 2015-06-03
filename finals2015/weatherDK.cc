#include <cstdio>
#include <iostream>
#include <queue>
using namespace std;

struct Group {
  double sz, p, ev;
  bool operator<(const Group& g) const {return p > g.p;}
};

int main() {
  int N;
  double P1, P2, P3, P4;
  while (cin >> N) {
    cin >> P1 >> P2 >> P3 >> P4;
    priority_queue<Group> q;
    for (int n1 = 0; n1 <= N; n1++)
    for (int n2 = 0; n1+n2 <= N; n2++)
    for (int n3 = 0; n1+n2+n3 <= N; n3++) {
      int n4 = N-n1-n2-n3;
      Group g;
      g.sz = 1.0;
      for (int i = 1; i <= N; i++) g.sz *= i;
      for (int i = 1; i <= n1; i++) g.sz /= i;
      for (int i = 1; i <= n2; i++) g.sz /= i;
      for (int i = 1; i <= n3; i++) g.sz /= i;
      for (int i = 1; i <= n4; i++) g.sz /= i;
      g.p = 1.0;
      for (int i = 0; i < n1; i++) g.p *= P1;
      for (int i = 0; i < n2; i++) g.p *= P2;
      for (int i = 0; i < n3; i++) g.p *= P3;
      for (int i = 0; i < n4; i++) g.p *= P4;
      g.ev = 0.0;
      if (g.p > 1e-90) q.push(g);
    }
    while (q.size() > 1) {
      Group g = q.top(); q.pop();
      if (g.sz < 1e15) {
        long long sz = (long long)(g.sz + 0.5);
        if (sz % 2) {
          sz--;
          Group g2 = q.top(); q.pop();
          g2.sz -= 1;
          if (g2.sz > 0.5) q.push(g2);
          Group g3;
          g3.sz = 1;
          g3.p = g.p + g2.p;
          g3.ev = g.ev + g2.ev + g3.p;
          q.push(g3);
        }
        g.sz = sz;
      }
      if (g.sz >= 2) {
        Group g2;
        g2.sz = g.sz / 2;
        g2.p = g.p * 2;
        g2.ev = g.ev * 2 + g2.p;
        q.push(g2);
      }
    }
    printf("%.6lf\n", q.top().ev);
  }
}
