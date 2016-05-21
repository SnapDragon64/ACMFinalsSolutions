#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int N, M, C;
vector<vector<int> > con, con2, dist, eidx;
vector<int> mindist, seen, ecap, ecap2;

bool flow(int x) {
  if (x == 1) return true;
  if (seen[x]) return false;
  seen[x] = true;
  for (int i = 0; i < con[x].size(); i++)
    if (ecap[eidx[x][i]]) {
      if (flow(con[x][i])) {
        ecap[eidx[x][i]] = 0;
        ecap[eidx[x][i]^1] = 1;
        return true;
      }
    }
  return false;
}

main() {
  int prob = 1;
  while (cin >> N >> M >> C) {
    con = dist = vector<vector<int> >(N+1);
    for (int i = 0; i < M; i++) {
      int x, y, t;
      cin >> x >> y >> t;
      con[x].push_back(y);
      dist[x].push_back(t);
      con[y].push_back(x);
      dist[y].push_back(t);
    }

    // Dijkstra
    mindist = vector<int>(N+1, 100000000);
    mindist[1] = 0;
    seen = vector<int>(N+1);
    for (int i = 0; i < N; i++) {
      int xd = 100000000, x = 0;
      for (int y = 1; y <= N; y++)
        if (!seen[y] && mindist[y] < xd) {
          xd = mindist[y];
          x = y;
        }
      assert(x > 0);
      seen[x] = true;
      for (int j = 0; j < con[x].size(); j++)
        mindist[con[x][j]] = min(mindist[con[x][j]], mindist[x] + dist[x][j]);
    }

    // Sort commuter distances.
    vector<pair<int, int> > comdist;
    for (int i = 0; i < C; i++) {
      int x;
      cin >> x;
      comdist.push_back(make_pair(mindist[x], x));
    }
    sort(comdist.begin(), comdist.end());

    // Keep only useful edges.
    con2 = eidx = vector<vector<int> >(N+1);
    ecap.clear();
    for (int x = 1; x <= N; x++)
    for (int j = 0; j < con[x].size(); j++)
      if (mindist[x] == mindist[con[x][j]] + dist[x][j]) {
        con2[x].push_back(con[x][j]);
        eidx[x].push_back(ecap.size());
        ecap.push_back(1);
        con2[con[x][j]].push_back(x);
        eidx[con[x][j]].push_back(ecap.size());
        ecap.push_back(0);
      }
    con = con2;
    ecap2 = ecap;

    // Find flow for each commuter.
    int ret = 0;
    for (int i = 0; i < comdist.size(); i++) {
      if (i && comdist[i-1].first != comdist[i].first) {
        ecap = ecap2;  // Reset flow graph
      }
      seen = vector<int>(N+1);
      ret += flow(comdist[i].second);
    }

    cout << "Case " << prob++ << ": " << ret << endl;
  }
}
