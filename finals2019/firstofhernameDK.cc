#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int N, K;
  while (cin >> N >> K) {
    vector<int> suffix(N+1, -1), succ(N+1);
    string S(N+1, -1);
    for (int i = 1; i <= N; i++) {
      cin >> S[i] >> succ[i];
      suffix[i] = S[i];
    }

    vector<int> succ2 = succ;
    for (int sz = 1; sz <= N; sz <<= 1) {
      vector<pair<pair<int, int>, int>> v(suffix.size());
      for (int i = N; i >= 0; i--) {
        v[i] = {{suffix[i], suffix[succ2[i]]}, i};
        succ2[i] = succ2[succ2[i]];
      }
      sort(v.begin(), v.end());
      for (int i = 0, cur = 0; i < v.size(); i++) {
        if (i > 0 && v[i-1].first != v[i].first) cur++;
        suffix[v[i].second] = cur;
      }
    }
    vector<pair<int, int>> order;
    for (int i = 0; i < suffix.size(); i++) order.emplace_back(suffix[i], i);
    sort(order.begin(), order.end());

    for (int i = 0; i < K; i++) {
      string s;
      cin >> s;
      auto cmp = [&] (int p) {
        for (int i = 0; i < s.size(); i++, p = succ[p]) {
          if (s[i] != S[p]) return s[i] < S[p] ? -1 : 1;
        }
        return 0;
      };
      int lo, hi;
      for (lo = 0, hi = N+1; lo < hi; ) {
        int mid = (lo+hi)/2;
        if (cmp(order[mid].second) > 0) lo = mid+1; else hi = mid;
      }
      int start = lo;
      for (lo = 0, hi = N+1; lo < hi; ) {
        int mid = (lo+hi)/2;
        if (cmp(order[mid].second) >= 0) lo = mid+1; else hi = mid;
      }
      cout << lo-start << endl;
    }
  }
}
