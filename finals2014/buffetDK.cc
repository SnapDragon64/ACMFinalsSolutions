#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

main() {
  int D, W;
  while (cin >> D >> W) {
    vector<pair<int, pair<int, int> > > MD;
    vector<pair<int, double> > MC;
    for (int i = 0; i < D; i++) {
      char ch;
      int w, t, dt;
      cin >> ch;
      if (ch == 'D') {
        cin >> w >> t >> dt;
        MD.push_back(make_pair(w, make_pair(t, dt)));
      } else {
        cin >> t >> dt;
        MC.push_back(make_pair(t, dt));
      }
    }

    vector<double> vc(W+1, -1e15);
    vc[0] = 0;
    double cw = 0.0, cv = 0.0;
    int cwi = 0;
    sort(MC.begin(), MC.end());
    while (cwi <= W && MC.size()) {
      while (MC.size() >= 2 && MC[MC.size()-2].first == MC[MC.size()-1].first) {
        MC[MC.size()-2].second = 1.0 / (1.0/MC[MC.size()-1].second +
                                        1.0/MC[MC.size()-2].second);
        MC.pop_back();
      }
      double curt = MC.back().first, curdt = MC.back().second;
      double nw = (curdt == 0.0) ? W+1 : cw + 1.0 / curdt;
      while (cwi <= W && cwi <= nw) {
        vc[cwi] = cv + (cwi-cw)*(curt + curt-(cwi-cw)*curdt)/2;
        cwi++;
      }
      cv += (nw-cw)*(curt + curt-(nw-cw)*curdt)/2;
      cw = nw;
      MC.back().first--;
    }

    vector<long long> vd(W+1, -1000000000000000LL);
    vd[0] = 0;
    random_shuffle(MD.begin(), MD.end());
    for (int i = 0; i < MD.size(); i++) {
      int w = MD[i].first, t = MD[i].second.first, dt = MD[i].second.second;
      for (int j = W-w; j >= 0; j--) {
        long long tt = t, ct = t;
        for (int k = j+w; k <= W; k += w) {
          if (vd[j] + tt <= vd[k]) break;
          vd[k] = vd[j] + tt;
          ct -= dt;
          tt += ct;
        }
      }
    }

    double ret = -1e15;
    for (int i = 0; i <= W; i++) ret = max(ret, vd[i] + vc[W-i]);
    if (ret < -5e14) {
      printf("impossible\n");
    } else {
      printf("%.9lf\n", ret);
    }
  }
}
