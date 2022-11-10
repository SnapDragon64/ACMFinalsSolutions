#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<vector<int>> fv, bv;
    int tot = 0;
    for (int i = 0; i < N; i++) {
      string S;
      cin >> S;
      int b = 0, mn = 0;
      for (int j = 0; j < S.size(); j++) {
        b += (S[j] == '(') - (S[j] == ')');
        mn = min(mn, b);
      }
      if (b >= 0) fv.push_back({-mn, b, i}); else bv.push_back({b-mn, -b, i});
      tot += b;
    }
    if (tot) goto fail;

    for (int i = 0; i < 2; i++) {
      auto& v = i ? fv : bv;
      sort(v.begin(), v.end());
      for (int j = 0, cur = 0; j < v.size(); j++) {
        if (cur < v[j][0]) goto fail;
        cur += v[j][1];
      }
    }

    reverse(bv.begin(), bv.end());
    for (auto const& v : fv) cout << v[2]+1 << endl;
    for (auto const& v : bv) cout << v[2]+1 << endl;
    continue;
fail:
    cout << "impossible" << endl;
  }
}
