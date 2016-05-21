#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, x, y;
  while (cin >> N) {
    vector<pair<int, pair<int, int>>> v;
    for (int i = 0; i < N; i++) {
      cin >> x >> y;
      v.push_back(make_pair((y>x) ? x : 2000000001-y, make_pair(x, y)));
    }

    long long ret = 0, cap = 0;
    sort(v.begin(), v.end());
    for (int i = 0; i < v.size(); i++) {
      if (cap < v[i].second.first) {
        ret += v[i].second.first - cap;
        cap = v[i].second.first;
      }
      cap += v[i].second.second - v[i].second.first;
    }

    cout << ret << endl;
  }
}
