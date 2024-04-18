#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int64_t N1, N2;
  while (cin >> N1) {
    vector<int> D1(N1);
    for (auto& x : D1) cin >> x;
    cin >> N2;
    vector<int> D2(N2);
    for (auto& x : D2) cin >> x;
    sort(D1.begin(), D1.end());
    sort(D2.begin(), D2.end());
    D1.push_back(2e9);
    D2.push_back(2e9);

    // Ensure D1 beats D2.
    int64_t prob = 0;
    for (int i1 = 0, i2 = 0, j2 = 0; i1 < N1; i1++) {
      while (D2[i2] < D1[i1]) i2++;
      while (D2[j2] <= D1[i1]) j2++;
      prob += 2*i2 + (j2-i2);
    }
    //cerr << "D1 beats D2 prob: " << prob/2.0/N1/N2 << endl;
    assert(prob != N1*N2);
    if (prob < N1*N2) { swap(N1, N2); swap(D1, D2); }

    vector<int> poss;
    for (auto x : D1) { if (x > 1) poss.push_back(x-1); poss.push_back(x); poss.push_back(x+1); }
    for (auto x : D2) { if (x > 1) poss.push_back(x-1); poss.push_back(x); poss.push_back(x+1); }
    sort(poss.begin(), poss.end());
    poss.erase(unique(poss.begin(), poss.end()), poss.end());
    while (poss.back() > 1.5e9) poss.pop_back();
    vector<pair<double, double>> v;
    for (int pi = 0, i1 = 0, i2 = 0, j1 = 0, j2 = 0; pi < poss.size(); pi++) {
      while (D1[i1] < poss[pi]) i1++;
      while (D2[i2] < poss[pi]) i2++;
      while (D1[j1] <= poss[pi]) j1++;
      while (D2[j2] <= poss[pi]) j2++;
      v.emplace_back((2*i1+(j1-i1)) / 2.0 / N1,
                     (2*i2+(j2-i2)) / 2.0 / N2);
    }

    for (int rep = 0; rep < 2; rep++) {
      vector<int> hull;
      for (int i = 0; i < v.size(); i++) {
        while (hull.size() >= 2) {
          auto [x1, y1] = v[hull[hull.size()-2]];
          auto [x2, y2] = v[hull[hull.size()-1]];
          auto [x3, y3] = v[i];
          if ((x3-x1)*(y2-y1) < (x2-x1)*(y3-y1)) break;
          hull.pop_back();
        }
        hull.push_back(i);
      }

      double ret = 1.0;
      for (int i = 0; i+1 < hull.size(); i++) {
        auto [x1, y1] = v[hull[i]];
        auto [x2, y2] = v[hull[i+1]];
        if (x1 >= 0.5 || x2 < 0.5) continue;
        ret = y1 + (y2-y1)/(x2-x1)*(0.5-x1);
      }
      if (!rep) cout << fixed << setprecision(9) << ret << ' '; else cout << 1-ret << endl;

      for (auto& [v1, v2] : v) { swap(v1, v2); v1 = 1-v1; v2 = 1-v2; }
      reverse(v.begin(), v.end());
      reverse(poss.begin(), poss.end());
    }
  }
}
