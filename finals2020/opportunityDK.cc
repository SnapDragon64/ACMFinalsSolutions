#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<int64_t> X(N), Y(N), Z(N);
    for (int i = 0; i < N; i++) cin >> X[i] >> Y[i] >> Z[i];
    vector<int64_t> ret(N);
    for (int b = 1; b < 7; b++) {
      int64_t mx = 0;
      for (int i = 0; i < N; i++) {
        int64_t cur = (b&1 ? X[i] : 0) + (b&2 ? Y[i] : 0) + (b&4 ? Z[i] : 0);
        mx = max(mx, cur);
      }
      for (int i = 0; i < N; i++) {
        int64_t cur = (b&1 ? X[i] : 0) + (b&2 ? Y[i] : 0) + (b&4 ? Z[i] : 0);
        ret[i] = max(ret[i], mx - cur);
      }
    }
    int64_t mn = 1e18, mni = 0;
    for (int i = 0; i < N; i++) {
      if (ret[i] < mn) { mn = ret[i]; mni = i; }
    }
    cout << mn << ' ' << mni+1 << endl;
  }
}
