#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int X, Y;
  cin >> Y >> X;
  vector<string> m(Y);
  for (auto& s : m) cin >> s;
  X += 2; Y += 2;
  int mx = max(X, Y);

  vector<string> g(2*mx+1, string(2*mx+1, '?'));
  const vector<int> dx{1, 0, -1, 0};
  const vector<int> dy{0, 1, 0, -1};
  int minx = mx, miny = mx, maxx = mx, maxy = mx;
  function<void(int,int,int)> rec = [&](int x, int y, int d) {
    minx = min(minx, x); maxx = max(maxx, x);
    miny = min(miny, y); maxy = max(maxy, y);
    g[y][x] = '.';
    int w;
    cin >> w;
    for (int i = 0; i < 4; i++, d = (d+1)&3) {
      if (g[y+dy[d]][x+dx[d]] == '?') {
        if (w) {
          cout << "step" << endl;
          rec(x+dx[d], y+dy[d], d);
          cout << "step" << endl; cin >> w;
          cout << "right" << endl; cin >> w;
          cout << "right" << endl; cin >> w;
        } else {
          g[y+dy[d]][x+dx[d]] = '#';
        }
      }
      cout << (i == 3 ? "left" : "right") << endl; cin >> w;
    }
  };
  rec(mx, mx, 0);

  int curx = mx, cury = mx;
  if ((minx+maxx)%2 == 0 && (miny+maxy)%2 == 0 && g[(miny+maxy)/2][(minx+maxx)/2] == '.') {
    vector<vector<vector<int>>> seen(2*mx+1, vector<vector<int>>(2*mx+1, vector<int>(4)));
    curx = (minx+maxx)/2; cury = (miny+maxy)/2;
    function<bool(int,int,int)> move = [&](int x, int y, int d) {
      if (seen[y][x][d] || g[y][x] == '#') return false;
      seen[y][x][d] = true;
      if (x == mx && y == mx && d == 2) return true;
      int w;
      if (move(x,       y,       (d+1)&3)) { cout << "left" << endl; cin >> w; return true; }
      if (move(x-dx[d], y-dy[d],  d     )) { cout << "step" << endl; cin >> w; return true; }
      return false;
    };
    move(curx, cury, 0);
  }

  int retx = -1, rety = -1;
  for (int y = 0; y < m   .size(); y++)
  for (int x = 0; x < m[y].size(); x++)
  for (int d = 0; d < 4; d++) {
    bool fail = false;
    for (int y2 = miny-1; !fail && y2 <= maxy+1; y2++)
    for (int x2 = minx-1; !fail && x2 <= maxx+1; x2++) if (g[y2][x2] != '?') {
      int x3 = x + dx[d]*(x2-curx) + dy[d]*(y2-cury);
      int y3 = y + dx[d]*(y2-cury) - dy[d]*(x2-curx);
      char ch = (x3 < 0 || x3 >= m[y].size() || y3 < 0 || y3 >= m.size() ? '#' : m[y3][x3]);
      fail = (g[y2][x2] != ch);
    }
    if (!fail) {
      if (retx != -1 && (retx != x || rety != y)) {
        retx = rety = -2;
      } else {
        retx = x; rety = y;
      }
    }
  }
  if (retx == -2) {
    cout << "no" << endl;
  } else {
    cout << "yes " << rety+1 << ' ' << retx+1 << endl;
  }
}
