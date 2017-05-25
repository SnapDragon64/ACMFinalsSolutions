#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> FAIL(1, "fail");

inline char chxor(char ch1, char ch2) {
  return (ch1 == '#') ^ (ch2 == '#') ? '#' : '.';
}

string invert_line(string s) {
  string ret;
  for (int i = 0; i+2 < s.size(); i++) {
    ret += s[i];
    s[i+1] = chxor(s[i], s[i+1]);
    s[i+2] = chxor(s[i], s[i+2]);
  }
  if (s[s.size()-2] != '.' || s[s.size()-1] != '.') return "";
  return ret;
}

vector<string> invert(vector<string> g) {
  int sx, sy;
  for (sy = 0; sy < g.size(); sy++) {
    if (invert_line(g[sy]).size() == 0) break;
  }
  if (sy < g.size()) {
    for (sx = 0; sx < g[0].size(); sx++) {
      string s;
      for (int y = 0; y < g.size(); y++) s += g[y][sx];
      if (invert_line(s).size() == 0) break;
    }
    if (sx == g[0].size()) return FAIL;
    g[sy][sx] = chxor('#', g[sy][sx]);
  }

  vector<string> ret(g.size()-2);
  for (int y = 0; y+2 < g.size(); y++) {
    ret[y] = invert_line(g[y]);
    if (ret[y].size() == 0) return FAIL;
    for (int y2 = y+1; y2 <= y+2; y2++)
    for (int x = 0; x < ret[y].size(); x++) {
      g[y2][x  ] = chxor(g[y2][x  ], ret[y][x]);
      g[y2][x+1] = chxor(g[y2][x+1], ret[y][x]);
      g[y2][x+2] = chxor(g[y2][x+2], ret[y][x]);
    }
  }
  if (g[g.size()-1].find('#') != -1 || g[g.size()-2].find('#') != -1) {
    return FAIL;
  }

  int minx = 1000000, maxx = -1, miny = 1000000, maxy = -1;
  for (int y = 0; y < ret.size(); y++)
  for (int x = 0; x < ret[0].size(); x++) if (ret[y][x] == '#') {
    minx = min(minx, x);
    maxx = max(maxx, x);
    miny = min(miny, y);
    maxy = max(maxy, y);
  }
  vector<string> ret2(maxy-miny+1);
  for (int y = miny; y <= maxy; y++) {
    ret2[y-miny] = ret[y].substr(minx, maxx-minx+1);
  }
  return ret2;
}

int main() {
  int X, Y;
  while (cin >> X >> Y) {
    vector<string> g(Y);
    for (int y = 0; y < Y; y++) cin >> g[y];

    while (g.size() >= 3 && g[0].size() >= 3) {
      vector<string> g2 = invert(g);
      if (g2 == FAIL) break;
      g = g2;
    }

    for (int y = 0; y < g.size(); y++) cout << g[y] << endl;
  }
}
