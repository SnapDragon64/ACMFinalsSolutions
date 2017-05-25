#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;
// Returns -1 for unmatched items.
// Complexity: O(V*E)
VI BipartiteMatch(const VVI &mat, VI *back_match = NULL) {
  int max_item = -1;
  VI fmat(mat.size(), -1), seen(mat.size(), -1), prev(mat.size());
  for (int i = 0; i < mat.size(); i++) if (mat[i].size())
    max_item = max(max_item, *max_element(mat[i].begin(), mat[i].end()));
  VI bmat(max_item+1, -1);

  for (int i = 0; i < mat.size(); i++) {
    VI q(1, i);
    seen[i] = i; prev[i] = -1;
    int x, y;
    while (!q.empty()) {
      x = q.back(); q.pop_back();
      for (VI::const_iterator it = mat[x].begin(); it != mat[x].end(); ++it) {
        int bm = bmat[*it];
        if (bm == -1) {y = *it; goto found_match;}
        if (seen[bm] < i) {
          seen[bm] = i; prev[bm] = x;
          q.push_back(bm);
        }
      }
    }
    continue;
found_match:
    while (x != -1) {
      bmat[y] = x;
      swap(y, fmat[x]);
      x = prev[x];
    }
  }

  if (back_match) *back_match = bmat;
  return fmat;
}

int main() {
  int X, Y;
  while (cin >> Y >> X) {
    VVI g(Y, vector<int>(X));
    VI xh(X), yh(Y);
    long long ret = 0;
    for (int y = 0; y < Y; y++)
    for (int x = 0; x < X; x++) {
      cin >> g[y][x];
      ret += max(0, g[y][x] - 1);
      xh[x] = max(xh[x], g[y][x]);
      yh[y] = max(yh[y], g[y][x]);
    }

    set<int> seen;
    for (int x = 0; x < X; x++) if (xh[x] > 1) {
      ret -= xh[x]-1;
      if (seen.count(xh[x])) continue;
      seen.insert(xh[x]);

      VVI mat;
      for (int x2 = 0; x2 < X; x2++) if (xh[x2] == xh[x]) {
        mat.push_back(vector<int>());
        for (int y2 = 0, yi = 0; y2 < Y; y2++) if (yh[y2] == xh[x]) {
          if (g[y2][x2]) mat.back().push_back(yi);
          yi++;
        }
      }
      VI v = BipartiteMatch(mat);
      for (int i = 0; i < v.size(); i++) if (v[i] != -1) ret += xh[x]-1;
    }
    for (int y = 0; y < Y; y++) ret -= max(0, yh[y]-1);

    cout << ret << endl;
  }
}
