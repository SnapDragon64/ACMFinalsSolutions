#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

bool slopecmp(const pair<int, pair<int, int> >& a,
              const pair<int, pair<int, int> >& b) {
  long long av = (long long)a.second.first * b.second.second;
  long long bv = (long long)b.second.first * a.second.second;
  if (av != bv) return av < bv;
  return a.first > b.first;
}

main() {
  int N;
  while (cin >> N) {
    vector<int> x1(N), x2(N), y(N);
    for (int i = 0; i < N; i++) {
      cin >> x1[i] >> x2[i] >> y[i];
      if (x1[i] > x2[i]) swap(x1[i], x2[i]);
    }
    int ret = 0;
    for (int i = 0; i < N; i++) {
      vector<pair<int, pair<int, int> > > v;
      int cur = 0;
      for (int j = 0; j < N; j++) {
        if (y[j] == y[i]) {
          if (x1[i] >= x1[j] && x1[i] <= x2[j]) {
            cur += x2[j]-x1[j];
          }
        } else if (y[j] < y[i]) {
          v.push_back(make_pair(x2[j]-x1[j], make_pair(x1[i]-x2[j], y[i]-y[j])));
          v.push_back(make_pair(x1[j]-x2[j], make_pair(x1[i]-x1[j], y[i]-y[j])));
        } else {
          v.push_back(make_pair(x2[j]-x1[j], make_pair(x1[j]-x1[i], y[j]-y[i])));
          v.push_back(make_pair(x1[j]-x2[j], make_pair(x2[j]-x1[i], y[j]-y[i])));
        }
      }
      sort(v.begin(), v.end(), slopecmp);
      ret = max(ret, cur);
      for (int j = 0; j < v.size(); j++) {
        cur += v[j].first;
        ret = max(ret, cur);
      }
    }
    cout << ret << endl;
  }
}
