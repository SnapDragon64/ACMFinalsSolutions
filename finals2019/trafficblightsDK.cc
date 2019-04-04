#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

#define SPLIT (5*7*8*9)

int Gcd(int a, int b) { return b ? Gcd(b, a%b) : a; }

int main() {
  int N;
  while (cin >> N) {
    vector<vector<int>> filters;
    vector<int> X(N), R(N), G(N), period(N), fn(N, -1);
    for (int i = 0; i < N; i++) {
      cin >> X[i] >> R[i] >> G[i];
      int period = (R[i]+G[i])/Gcd(SPLIT, R[i]+G[i]);
      for (int j = 0; j < filters.size(); j++) {
        if (period % filters[j].size() == 0) filters[j].resize(period, -1);
        if (filters[j].size() % period == 0) {
          fn[i] = j;
          break;
        }
      }
      if (fn[i] == -1) {
        fn[i] = filters.size();
        filters.emplace_back(period, -1);
      }
    }

    vector<double> ret(N+1);
    for (int base = 0; base < SPLIT; base++) {
      double cur = 1.0/SPLIT;
      for (int i = 0; i < N; i++) {
        vector<int>& filter = filters[fn[i]];
        int tot = 0, filtered = 0;
        for (int j = 0, t = SPLIT+base+X[i]; j < filter.size(); j++, t += SPLIT) {
          if (filter[j] < base) {
            tot++;
            if (t%(R[i]+G[i]) < R[i]) {
              filter[j] = base;
              filtered++;
            }
          }
        }
        double oldCur = cur;
        if (tot) cur *= (double)(tot-filtered) / tot;
        ret[i] += oldCur-cur;
      }
      ret[N] += cur;
    }

    for (auto p : ret) printf("%.9lf\n", p);
  }
}
