#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, Q, U, V, W, S, K, T;
  while (cin >> N >> Q) {
    vector<vector<pair<int,int>>> c(N);
    int64_t tot = 0;
    for (int i = 0; i < N-1; i++) {
      cin >> U >> V >> W;
      U--; V--;
      c[U].push_back({V, W});
      c[V].push_back({U, W});
      tot += W;
    }

    vector<int> depth(N);
    vector<vector<pair<int64_t,int>>> longest(N);
    function<int64_t(int,int,int)> doLongest = [&](int x, int prev, int dp) {
      depth[x] = dp;
      int64_t ret = 0;
      for (auto [y, d] : c[x]) {
        longest[x].push_back({y == prev ? -1 : d+doLongest(y, x, dp+1), y});
        ret = max(ret, longest[x].back().first);
      }
      return ret;
    };
    doLongest(0, -1, 0);
    function<int64_t(int,int,int)> getLongest = [&](int x, int ex1, int ex2) -> int64_t {
      for (auto [l, y] : longest[x]) {
        if (y != ex1 && y != ex2) return l;
      }
      return 0;
    };
    function<void(int,int,int64_t)> doParLongest = [&](int x, int prev, int64_t parLongest) {
      for (auto& [l, _] : longest[x]) if (l == -1) l = parLongest;
      sort(longest[x].begin(), longest[x].end(), greater<pair<int64_t,int>>());
      for (auto [y, d] : c[x]) if (y != prev) doParLongest(y, x, d + getLongest(x, y, -1));
    };
    doParLongest(0, -1, 0);

    vector<vector<int>> skipNd(N);  // skip-paths going up the tree of length 2^n
    vector<vector<int>> skipPrev(N);  // first node on the skip-path going down
    vector<vector<int64_t>> skipSUp(N);  // max path-to-endpoint, starting at bottom, maybe entering interior subtree
    vector<vector<int64_t>> skipSDn(N);  // same, but starting at top instead
    vector<vector<int64_t>> skipKUp(N);  // skipSUp, but costs along the skip path are subtracted, not added
    vector<vector<int64_t>> skipKDn(N);  // same, but starting at top instead
    vector<vector<int64_t>> skipDist(N);  // total length of skip
    function<void(int,int,int64_t)> doSkip = [&](int x, int prev, int64_t d) {
      skipNd[x].push_back(prev);
      skipPrev[x].push_back(x);
      skipDist[x].push_back(d);
      skipSUp[x].push_back(d);
      skipSDn[x].push_back(d);
      skipKUp[x].push_back(0);
      skipKDn[x].push_back(0);
      for (int b = 1; (depth[x]&((1<<b)-1)) == 0; b++) {
        int y = skipNd[x][b-1];
        skipNd[x].push_back(skipNd[y][b-1]);
        skipPrev[x].push_back(skipPrev[y][b-1]);
        skipDist[x].push_back(skipDist[x][b-1] + skipDist[y][b-1]);
        int64_t ymx = getLongest(y, skipPrev[x][b-1], skipNd[y][0]);
        skipSUp[x].push_back(max(skipSUp[x][b-1],  skipDist[x][b-1] + max(ymx, skipSUp[y][b-1])));
        skipSDn[x].push_back(max(skipSDn[y][b-1],  skipDist[y][b-1] + max(ymx, skipSDn[x][b-1])));
        skipKUp[x].push_back(max(skipKUp[x][b-1], -skipDist[x][b-1] + max(ymx, skipKUp[y][b-1])));
        skipKDn[x].push_back(max(skipKDn[y][b-1], -skipDist[y][b-1] + max(ymx, skipKDn[x][b-1])));
      }
      for (int i = 0; i < c[x].size(); i++) if (c[x][i].first != prev) doSkip(c[x][i].first, x, c[x][i].second);
    };
    for (int i = 0; i < c[0].size(); i++) doSkip(c[0][i].first, 0, c[0][i].second);

    auto anc = [&](int x, int y) {
      vector<pair<int,int>> ret;
      while (depth[y] > depth[x]) {
        for (int b = skipNd[y].size()-1; b >= 0; b--) if (depth[y]-(1<<b) >= depth[x]) {
          y = skipNd[y][b];
          break;
        }
      }
      while (depth[x] > depth[y]) {
        for (int b = skipNd[x].size()-1; b >= 0; b--) if (depth[x]-(1<<b) >= depth[y]) {
          ret.push_back({x, b});
          x = skipNd[x][b];
          break;
        }
      }
      while (x != y) {
        for (int b = skipNd[x].size()-1; b >= 0; b--) if (b == 0 || skipNd[x][b] != skipNd[y][b]) {
          ret.push_back({x, b});
          x = skipNd[x][b]; y = skipNd[y][b];
          break;
        }
      }
      ret.push_back({x, -1});
      return ret;
    };

    for (int q = 0; q < Q; q++) {
      cin >> S >> K >> T;
      S--; K--; T--;

      auto sk = anc(S, K), st = anc(S, T), ks = anc(K, S), kt = anc(K, T);
      auto path1 = (depth[sk.back().first] > depth[st.back().first] ? sk : st);
      auto path2 = anc(path1.back().first, K);
      auto path4 = (depth[ks.back().first] > depth[kt.back().first] ? ks : kt);
      auto path3 = anc(path4.back().first, S);
      if (path1.back().first == T || path4.back().first == T) { cout << "impossible" << endl; continue; }

      int x = S, prev = -1;
      int64_t base = 0, ret = 0;
      for (int i = 0; i+1 < path1.size(); i++) {  // path1 rises from S, not overlapping path T->K
        auto [y, b] = path1[i];
        ret = max(ret, base + getLongest(x, prev, skipNd[y][0]));
        ret = max(ret, base + skipSUp[y][b]);
        base += skipDist[y][b];
        prev = skipPrev[y][b];
        x = skipNd[y][b];
      }
      for (int i = 0; i+1 < path2.size(); i++) {  // path2 rises from path1, overlapping path T->K
        auto [y, b] = path2[i];
        ret = max(ret, base + getLongest(x, prev, skipNd[y][0]));
        ret = max(ret, base + skipKUp[y][b]);
        base -= skipDist[y][b];
        prev = skipPrev[y][b];
        x = skipNd[y][b];
      }
      for (int i = path3.size()-2; i >= 0; i--) {  // path3 descends from path2, not overlapping path T->K
        auto [y, b] = path3[i];
        ret = max(ret, base + getLongest(x, prev, skipPrev[y][b]));
        ret = max(ret, base + skipSDn[y][b]);
        base += skipDist[y][b];
        prev = skipNd[y][0];
        x = y;
      }
      for (int i = path4.size()-2; i >= 0; i--) {  // path4 descends from path3, overlapping path T->K
        auto [y, b] = path4[i];
        ret = max(ret, base + getLongest(x, prev, skipPrev[y][b]));
        ret = max(ret, base + skipKDn[y][b]);
        base -= skipDist[y][b];
        prev = skipNd[y][0];
        x = y;
      }
      ret = max(ret, base + getLongest(x, prev, -1));
      cout << 2*tot - ret << endl;
    }
  }
}
