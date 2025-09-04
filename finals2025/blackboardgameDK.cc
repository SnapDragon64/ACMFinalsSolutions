#include <algorithm>
#include <cassert>
#include <functional>
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
  for (auto row : mat) if (row.size()) max_item = max(max_item, *max_element(row.begin(), row.end()));
  VI bmat(max_item+1, -1);

  for (int i = 0; i < mat.size(); i++) {
    VI q(1, i);
    seen[i] = i; prev[i] = -1;
    int x, y;
    while (!q.empty()) {
      x = q.back(); q.pop_back();
      for (auto it : mat[x]) {
        int bm = bmat[it];
        if (bm == -1) {y = it; goto found_match;}
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

  if (back_match) back_match->swap(bmat);
  return fmat;
}

vector<bool> prime;
void PrimeSieve(int limit) {
  prime = vector<bool>(limit+1, true);
  prime[0] = prime[1] = false;
  for (int x = 2; x*x <= limit; x++) if (prime[x]) {
    for (int y = x*x; y <= limit; y += x) prime[y] = false;
  }
}

int main() {
  PrimeSieve(10000000);

  int T, N;
  for (cin >> T; T--;) {
    cin >> N;
    if (N > 83) {
      for (int p = N/3; ; p--) if (prime[p]) {
        cout << "first " << 2*p << endl;
        break;
      }
    } else {
      int ret;
      for (ret = 2; ret <= N; ret += 2) {
        VVI mat(N+1);
        for (int x = 1; x <= N; x++) {
          for (int y = 1; y <  x; y++) if (x%y == 0 && prime[x/y]) mat[x].push_back(y);
          for (int y = x; y <= N; y++) if (y%x == 0 && prime[y/x]) mat[x].push_back(y);
        }
        int cnt = 0;
        for (auto x : BipartiteMatch(mat)) cnt += (x != -1);
        mat[ret].clear();
        for (auto x : BipartiteMatch(mat)) cnt -= (x != -1);
        if (cnt == 0) break;
      }
      if (ret <= N) cout << "first " << ret << endl; else cout << "second" << endl;
    }
  }
}
