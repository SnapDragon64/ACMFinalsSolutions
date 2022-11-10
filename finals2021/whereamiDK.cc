#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int X, Y;
  while (cin >> X >> Y) {
    vector<string> g(Y);
    for (int y = Y-1; y >= 0; y--) cin >> g[y];

    vector<vector<int>> dist(201, vector<int>(201));
    for (int x = 100, y = 100, dx = 0, dy = 1, step = 0, stepn = 1, cur = 0; y < 201; cur++) {
      dist[y][x] = cur;
      x += dx; y += dy;
      if (++step == stepn) {
        swap(dx, dy);
        dy = -dy;
        step = 0;
        if (dy) stepn++;
      }
    }

    vector<vector<int>> obs(40001);
    for (int y = 0; y < Y; y++)
    for (int x = 0; x < X; x++) if (g[y][x] == 'X')
    for (int sy = 0, i = 0; sy < Y; sy++)
    for (int sx = 0; sx < X; sx++, i++) {
      obs[dist[y-sy+100][x-sx+100]].push_back(i);
    }

    vector<int> comp(X*Y), compt(X*Y), compsz{X*Y};
    for (int t = 0; compsz.size() < X*Y; t++) if (obs[t].size()) {
      vector<int>& v = obs[t];
      sort(v.begin(), v.end(), [&](int x, int y) { return comp[x] < comp[y]; });
      for (int i = 0, j = 0; i < v.size(); i = j) {
        for (j++; j < v.size() && comp[v[j]] == comp[v[i]]; j++)
          ;
        int& sz = compsz[comp[v[i]]];
        if (j-i == sz) continue;
        if (j-i == 1) compt[compsz.size()] = t;
        sz -= j-i;
        if (sz == 1) compt[comp[v[i]]] = t;
        for (int k = i; k < j; k++) comp[v[k]] = compsz.size();
        compsz.push_back(j-i);
      }
    }

    int mx = 0, tot = 0;
    for (int i = 0; i < X*Y; i++) {
      mx = max(mx, compt[i]);
      tot += compt[i];
    }

    cout << fixed << setprecision(9) << double(tot)/X/Y << endl;
    cout << mx << endl;
    bool first = true;
    for (int i = 0; i < X*Y; i++) if (compt[comp[i]] == mx) {
      if (!first) cout << ' ';
      first = false;
      cout << '(' << i%X+1 << ',' << i/X+1 << ')';
    }
    cout << endl;
  }
}
