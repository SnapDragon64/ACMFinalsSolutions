#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

int main() {
  int N, W;
  while (cin >> N >> W) {
    vector<string> D(N);
    for (auto& d : D) cin >> d;

    map<string, vector<int>> dw;
    function<void(int,int,string)> doit = [&](int d, int b, string s) {
      if (d == N) {
        sort(s.begin(), s.end());
        dw[s].push_back(b);
        return;
      }
      for (int i = 0; i < D[d].size(); i++) doit(d+1, b + ((i+1)<<(3*d)), s+D[d][i]);
    };
    doit(0, 0, "");

    vector<int> curn(1<<(3*N)), seen(1<<(3*N));
    vector<double> cure(1<<(3*N)), beste(1<<(3*N), 1e9);
    priority_queue<pair<double, int>> q;
    // For a solved full configuration, adjust the expected values of partial configurations that may roll it.
    function<void(int,int,int,double)> sete = [&](int d, int pw, int b, double e) {
      if (d == N) {
        if (pw == 1) return;
        curn[b]++;
        cure[b] += e;
        // We know that be = 1 + ((pw-curn) / pw) * be + (curn / pw) * (cure/curn).
        double be = (pw + cure[b]) / curn[b];
        beste[b] = be;
        q.push({-be, b});
        return;
      }
      sete(d+1, pw, b, e);
      sete(d+1, pw*D[d].size(), b & ~(7<<(3*d)), e);
    };
    // For a solved partial configuration, any unsolved full configurations that can reach it are now solved.
    function<void(int,int,double)> brec = [&](int d, int b, double e) {
      if (d == N) {
        if (!seen[b]) sete(0, 1, b, e);
        seen[b] = true;
        return;
      }
      if (b&(7<<(3*d))) {
        brec(d+1, b, e);
      } else {
        for (int i = 0; i < D[d].size(); i++) brec(d+1, b + ((i+1)<<(3*d)), e);
      }
    };

    for (int i = 0; i < W; i++) {
      string w;
      cin >> w;
      sort(w.begin(), w.end());
      for (auto b : dw[w]) brec(0, b, 0.0);
    }

    if (q.empty()) { cout << "impossible" << endl; continue; }
    while (!q.empty()) {
      auto [e, b] = q.top(); q.pop(); e = -e;
      if (seen[b]) continue;
      seen[b] = true;
//for (int d = 0; d < N; d++) if (b&(7<<(3*d))) cout << D[d][((b>>(3*d))&7)-1]; else cout << '.';
//cout << ": " << e << endl;
      // Configuration b is now solved.
      brec(0, b, e);
    }

    cout << fixed << setprecision(9) << beste[0] << endl;
  }
}
