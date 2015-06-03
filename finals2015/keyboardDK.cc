#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

int dx[4] = {1, 0, -1, 0}, dy[4] = {0, -1, 0, 1};
int mv[50][50][4];
int best[50][50];

main() {
  int Y, X;
  while (cin >> Y >> X) {
    vector<string> g(Y);
    for (int i = 0; i < Y; i++) cin >> g[i];
    string message;
    cin >> message;
    message += '*';

    memset(mv, -1, sizeof(mv));
    for (int y = 0; y < Y; y++)
    for (int x = 0; x < X; x++)
    for (int d = 0; d < 4; d++) {
      int x2 = x, y2 = y;
      for (int n = 0; ; n++) {
        if (x2 < 0 || x2 >= X || y2 < 0 || y2 >= Y) break;
        if (g[y2][x2] != g[y][x]) {
          mv[y][x][d] = n;
          break;
        }
        x2 += dx[d]; y2 += dy[d];
      }
    }

    vector<pair<int, pair<short, short> > > cur;
    cur.push_back(make_pair(0, make_pair(0, 0)));
    for (int i = 0; i < message.size(); i++) {
      memset(best, 63, sizeof(best));
      vector<pair<short, short> > v;
      int dist = 0, curi = 0;
      sort(cur.begin(), cur.end());
      for (;;) {
        if (v.empty()) {
          if (curi == cur.size()) break;
          dist = cur[curi].first;
        }
        while (curi < cur.size() && cur[curi].first == dist) {
          v.push_back(cur[curi++].second);
        }
        vector<pair<short, short> > v2;
        for (int j = 0; j < v.size(); j++) {
          int x = v[j].first, y = v[j].second;
          if (best[y][x] <= dist) continue;
          best[y][x] = dist;
          for (int d = 0; d < 4; d++) if (mv[y][x][d] != -1) {
            int x2 = x + dx[d] * mv[y][x][d], y2 = y + dy[d] * mv[y][x][d];
            if (best[y2][x2] > dist+1) {
              v2.push_back(make_pair(x2, y2));
            }
          }
        }
        v.swap(v2);
        dist++;
      }

      cur.clear();
      for (int y = 0; y < Y; y++)
      for (int x = 0; x < X; x++) if (g[y][x] == message[i]) {
        cur.push_back(make_pair(best[y][x]+1, make_pair(x, y)));
      }
    }

    int ret = 1000000000;
    for (int i = 0; i < cur.size(); i++) ret = min(ret, cur[i].first);
    cout << ret << endl;
  }
}
