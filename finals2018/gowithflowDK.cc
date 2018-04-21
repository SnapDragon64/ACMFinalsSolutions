#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    int mn = 0;
    vector<int> v;
    for (int i = 0; i < N; i++) {
      string s;
      cin >> s;
      v.push_back(s.size());
      mn = max(mn, v.back());
    }

    int ret = 0, retw = -1;
    for (int w = mn; ; w++) {
      vector<int> cur(w), next(w);
      int cs = -1, nline = 1;
      for (int vi = 0; vi < v.size(); vi++) {
        if (cs+v[vi] >= w) {
          cur.swap(next);
          next = vector<int>(w);
          cs = -1;
          nline++;
        }
        if (cs >= 0) {
          next[cs] = max(cur[cs-1]+1, max(cur[cs]+1, cur[cs+1]+1));
          if (next[cs] > ret) { ret = next[cs]; retw = w; }
        }
        cs += v[vi]+1;
      }
      if (ret >= nline) break;
    }
    cout << retw << ' ' << ret << endl;
  }
}
