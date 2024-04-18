#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

template<typename T> constexpr T Gcd(const T& a, const T& b) { return b != 0 ? Gcd(b, a%b) : a < 0 ? -a : a; }

void tilt(int dir, vector<vector<int>>& g) {
  int X = g[0].size(), Y = g.size();
  if (dir&1) swap(X, Y);
  auto get = [&](int x, int y) {
    return dir == 0 ? &g[y][x] : dir == 1 ? &g[x][y] : dir == 2 ? &g[y][X-1-x] : &g[X-1-x][y];
  };
  for (int y = 0; y < Y; y++) {
    int x2 = 0;
    for (int x = 0; x < X; x++) if (*get(x, y)) {
      *get(x2++, y) = *get(x, y);
    }
    for (; x2 < X; x2++) *get(x2, y) = 0;
  }
}

pair<int64_t, int64_t> match(const string& s, const string& t) {
  // Who needs hashing/string algorithms?
  int64_t r, m;
  for (r = 0; r <= s.size(); r++) {
    if (r == s.size()) return {0, 0};
    if (memcmp(&s[r], &t[0], s.size()-r) == 0 && memcmp(&s[0], &t[s.size()-r], r) == 0) break;
  }
  for (m = r ? r : 1; m < s.size(); m++) {
    if (s.size() % m == 0 && memcmp(&s[0], &s[m], s.size()-m) == 0) break;
  }
  return {r, m};
}

int main() {
  int Y, X;
  while (cin >> Y >> X) {
    vector<vector<int>> g(Y, vector<int>(X)), g2 = g;
    char ch;
    for (auto& row : g ) for (auto& v : row) { cin >> ch; if (ch != '.') v = ch-'a'+1; }
    for (auto& row : g2) for (auto& v : row) { cin >> ch; if (ch != '.') v = ch-'a'+1; }

    for (int sd = 0; sd < 4; sd++)
    for (int dd = 1; dd < 4; dd += 2) {
      auto tg = g;
      for (int i = 0, d = sd; i <= 6; i++, d = (d+dd)%4) {
        if (tg == g2) goto pass;
        if (i >= 2) {
          auto ng = tg;
          for (int y = 0; y < Y; y++)
          for (int x = 0; x < X; x++) {
            if (!!g2[y][x] != !!ng[y][x]) goto nomatch;
            if (ng[y][x]) ng[y][x] = y*X+x+1;
          }
          for (int j = 0; j < 4; j++) tilt((d+j)%4, ng);
          vector<int64_t> residues, mods;
          for (int y = 0; y < Y; y++)
          for (int x = 0; x < X; x++) if (ng[y][x]) {
            string s, t;
            for (int* ptr = &ng[y][x]; *ptr;) {
              int x2 = (*ptr-1)%X, y2 = (*ptr-1)/X;
              *ptr = 0;
              s += tg[y2][x2]; t += g2[y2][x2];
              ptr = &ng[y2][x2];
            }

            auto [residue, mod] = match(s, t);
            if (mod == 0) goto nomatch;
            if (mod == 1) continue;
            for (int i = 0; i < mods.size(); i++) {
              int64_t g = Gcd(mod, mods[i]);
              if (residues[i] % g != residue % g) goto nomatch;
            }
            //cerr << "Adding r=" << residue << " m=" << mod << endl;
            residues.push_back(residue); mods.push_back(mod);
          }
          goto pass;
        }
nomatch:;
        tilt(d, tg);
      }
    }

fail:
    cout << "no" << endl;
    continue;
pass:
    cout << "yes" << endl;
  }
}
