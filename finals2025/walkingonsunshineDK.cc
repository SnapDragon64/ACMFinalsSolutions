#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, foo, Yc, Ya, Y1, Y2;
  while (cin >> N >> foo >> Yc >> foo >> Ya) {
    vector<pair<int, int>> v;
    for (int i = 0; i < N; i++) {
      cin >> foo >> Y1 >> foo >> Y2;
      v.push_back({Y1, Y2});
    }
    sort(v.begin(), v.end());
    int y = Ya, ret = 0;
    for (auto [y1, y2] : v) {
      ret += max(0, min(Yc, y1) - y);
      y = max(y, y2);
    }
    ret += max(0, Yc - y);
    cout << ret << endl;
  }
}
