#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

main() {
  int N, W, T1, T2;
  double U, V;
  while (cin >> N >> W >> U >> V >> T1 >> T2) {
    vector<pair<double, int> > v;
    v.push_back(make_pair(T1, 0));
    v.push_back(make_pair(T2, 0));
    for (int i = 0; i < N; i++) {
      char ch;
      int M, L, P;
      cin >> ch >> M;
      for (int j = 0; j < M; j++) {
        cin >> L >> P;
        if (ch == 'W') P = -P;
        v.push_back(make_pair(-P/U - W*(i+1)/V, 1));
        v.push_back(make_pair(-(P-L)/U - W*i/V, -1));
      }
    }
    sort(v.begin(), v.end());

    double ret = 0.0;
    int cur = 0;
    for (int i = 0; i < v.size(); i++) {
      cur += v[i].second;
      if (cur == 0 && v[i].first >= T1 && v[i].first < T2) {
        ret = max(ret, v[i+1].first-v[i].first);
      }
    }
    assert(ret > 0.101);
    printf("%.3lf\n", ret);
  }
}
