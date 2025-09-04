#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

#define EPS 1e-9

int main() {
  // NOTE: This is not the easiest way to solve this problem.  The original problem
  // was posed with R <= 10, so ternary searches would not work.
  int S, R, D;
  while (cin >> S >> R >> D) {
    vector<vector<vector<pair<int, int>>>> ducts(S+1);
    for (int i = 0; i < D; i++) {
      int I, N;
      cin >> I >> N;
      vector<pair<int, int>> d(N);
      for (auto& [O, P] : d) cin >> O >> P;
      ducts[I].push_back(d);
    }

    auto solve = [&](const vector<long double>& dir) {
      vector<long double> v(S+R+1), flow(S+R+1);
      vector<int> d(S+1);
      for (int i = 0; i < dir.size(); i++) v[S+1+i] = max(dir[i], 0.0L);
      for (int i = S; i >= 1; i--)
      for (int j = 0; j < ducts[i].size(); j++) {
        long double cur = 0.0;
        for (auto [O, P] : ducts[i][j]) cur += v[O]*P;
        cur /= 100;
        if (cur > v[i]) { v[i] = cur; d[i] = j; }
      }
      flow[1] = 1;
      for (int i = 1; i <= S; i++) {
        flow[i] /= 100;
        for (auto [O, P] : ducts[i][d[i]]) flow[O] += flow[i]*P;
      }
      for (int i = 0; i < dir.size(); i++) if (v[S+1+i] == 0.0) flow[S+1+i] = 0.0;  // Can "drop" flow.
      return vector<long double>(flow.begin()+S+1, flow.end());
    };

    auto inner = [&](const vector<long double>& a, const vector<long double>& b) {
      long double ret = 0.0;
      for (int i = 0; i < R; i++) ret += a[i]*b[i];
      return ret;
    };
    auto normal = [&](const vector<vector<long double>>& vertices) {
      for (;;) {
        vector<vector<long double>> v(R, vector<long double>(R));
        for (int i = 1; i < R; i++) for (int j = 0; j < R; j++) v[i-1][j] = vertices[i][j] - vertices[0][j];
        for (auto& x : v[R-1]) x = rand()%101;
        for (int i = 1; i < R; i++)
        for (int j = 0; j < i; j++) {
          long double dp = inner(v[j], v[i]), lensq = inner(v[j], v[j]);
          for (int k = 0; k < R; k++) v[i][k] -= v[j][k] * dp / lensq;
        }
        long double len = sqrt(inner(v[R-1], v[R-1]));
        if (abs(len) > 1e-3) {
          for (auto& x : v[R-1]) x /= len;
          return v[R-1];
        }
      }
    };

    long double ret = 0.0;
    vector<long double> mindir(R, 1.0);
    set<vector<vector<long double>>> seen;
    function<void(vector<vector<long double>>)> rec = [&](vector<vector<long double>> vertices) {
      sort(vertices.begin(), vertices.end());
      if (!seen.insert(vertices).second) return;

      vector<long double> dir = normal(vertices);
      if (inner(dir, vertices[0]) < 0) for (auto& x : dir) x = -x;
      long double probev = inner(dir, vertices[0]) / inner(dir, mindir);
      ret = max(ret, probev);
      vector<long double> newv = solve(dir);
      if (inner(dir, newv) / inner(dir, mindir) < ret+EPS) return;

      vector<vector<long double>> norms;
      for (int i = 0; i < R; i++) {
        newv.swap(vertices[i]);
        vector<long double> norm = normal(vertices);
        if (inner(norm, newv) < inner(norm, vertices[i])) for (auto& x : norm) x = -x;
        norms.push_back(norm);
        newv.swap(vertices[i]);
      }

      long double mx = probev;
      vector<int> mxi;
      for (int i = 0; i < R; i++) {
        if (abs(inner(norms[i], mindir)) < EPS) continue;
        vector<long double> inter(R, inner(norms[i], newv) / inner(norms[i], mindir));
        bool fail = false;
        for (int j = 0; j < R; j++) if (inner(norms[j], inter) < inner(norms[j], newv)-EPS) fail = true;
        if (inter[0] < mx-EPS) continue;
        if (fail) continue;
        if (inter[0] > mx+EPS) mxi.clear();
        mx = max(mx, inter[0]);
        mxi.push_back(i);
      }
      for (auto i : mxi) {
        newv.swap(vertices[i]);
        rec(vertices);
        newv.swap(vertices[i]);
      }
    };

    vector<vector<long double>> vertices(R, vector<long double>(R));
    for (int i = 0; i < R; i++) {
      vertices[i][i] = 1.0;
      vertices[i][i] = solve(vertices[i])[i];
      if (vertices[i][i] < EPS) goto done;
    }
    rec(vertices);

done:
    cout << fixed << setprecision(8) << ret*100 << endl;
  }
}
