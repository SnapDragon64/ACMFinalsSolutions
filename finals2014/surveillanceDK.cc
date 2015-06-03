#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int N, K;
vector<pair<int, int> > c;
int skip[1000001][20];

int dist(int s, int e) {
  if (s >= e) return 0;
  int i = lower_bound(c.begin(), c.end(),
      make_pair(s, 1000000000)) - c.begin() - 1;
  if (i < 0) return 1000000000;
  int ret = 1;
  for (int skp = 19; skp >= 0; skp--) {
    if (c[skip[i][skp]].second+1 < e) {
      ret += (1<<skp);
      i = skip[i][skp];
    }
  }
  if (c[i].second+1 < e) {
    ret++;
    i = skip[i][0];
  }
  if (c[i].second+1 < e) return 1000000000;
  return ret;
}

main() {
  while (cin >> N >> K) {
    c.clear();
    vector<pair<int, int> > v;
    for (int i = 0; i < K; i++) {
      int a, b;
      cin >> a >> b;
      if (a <= b) {
        c.push_back(make_pair(a, b));
      } else {
        v.push_back(make_pair(a, b));
        c.push_back(make_pair(1, b));
      }
    }

    c.push_back(make_pair(N+1, N+1));
    sort(c.begin(), c.end());
    {
      vector<pair<int, int> > c2;
      for (int i = 0, curmx = 0; i < c.size(); i++) {
        if (c[i].second <= curmx) continue;
        if (i+1 < c.size() && c[i+1].first == c[i].first) continue;
        c2.push_back(c[i]);
        curmx = c[i].second;
      }
      c = c2;
    }

    for (int i = 0; i < c.size(); i++) {
      skip[i][0] = lower_bound(c.begin(), c.end(),
          make_pair(c[i].second+1, 1000000000)) - c.begin() - 1;
    }
    for (int skp = 1; skp < 20; skp++)
    for (int i = 0; i < c.size(); i++) {
      skip[i][skp] = skip[skip[i][skp-1]][skp-1];
    }

/*for (int i = 0; i < c.size(); i++) {
cout << i << ' ' << c[i].first << '-' << c[i].second << ":";
for (int j = 0; j < 20; j++) cout << ' ' << skip[i][j];
cout << endl;
}*/

    int ret = dist(1, N+1);
    for (int i = 0; i < v.size(); i++) {
      ret = min(ret, 1 + dist(v[i].second+1, v[i].first));
    }
    if (ret == 1000000000) {
      cout << "impossible" << endl;
    } else {
      cout << ret << endl;
    }
  }
}
