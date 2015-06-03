#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

main() {
  int N;
  while (cin >> N) {
    vector<vector<int> > c(2*N, vector<int>(2*N));
    for (int i = 0; i < 2*N; i++) c[i][i] = 1000000001;
    vector<pair<int, pair<int, int> > > e;
    for (int i = 0; i < N; i++)
    for (int j = i+1; j < N; j++) {
      int d;
      cin >> d;
      c[i][j+N] = max(c[i][j+N], d);
      c[j][i+N] = max(c[j][i+N], d);
      e.push_back(make_pair(d, make_pair(i, j)));
    }
    if (N <= 2) {cout << 0 << endl; continue;}

    for (int k = 0; k < 2*N; k++)
    for (int i = 0; i < 2*N; i++)
    for (int j = 0; j < 2*N; j++) {
      c[i][j] = max(c[i][j], min(c[i][k], c[k][j]));
    }

    sort(e.begin(), e.end());
    int ret = e.back().first;
    for (int i = e.size()-1; i >= 0; i--) {
      int mx = 0;
      for (int j = 0; j < N; j++) mx = max(mx, min(c[j][j+N], c[j+N][j]));
      ret = min(ret, mx + e[i].first);
      int a = e[i].second.first, b = e[i].second.second;
      if (c[a+N][b] < 1000000001)
      for (int j = 0; j < 2*N; j++)
      for (int k = 0; k < 2*N; k++) {
        c[j][k] = max(c[j][k], min(c[j][a+N], c[b][k]));
      }
      if (c[a][b+N] < 1000000001)
      for (int j = 0; j < 2*N; j++)
      for (int k = 0; k < 2*N; k++) {
        c[j][k] = max(c[j][k], min(c[j][a], c[b+N][k]));
      }
      if (c[b+N][a] < 1000000001)
      for (int j = 0; j < 2*N; j++)
      for (int k = 0; k < 2*N; k++) {
        c[j][k] = max(c[j][k], min(c[j][b+N], c[a][k]));
      }
      if (c[b][a+N] < 1000000001)
      for (int j = 0; j < 2*N; j++)
      for (int k = 0; k < 2*N; k++) {
        c[j][k] = max(c[j][k], min(c[j][b], c[a+N][k]));
      }
    }

    cout << ret << endl;
  }
}
