#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  int N;
  while (cin >> N) {
    vector<vector<pair<int64_t, int64_t>>> v(2);
    for (int i = 0; i < 2; i++)
    for (int j = 0; j < N; j++) {
      double A, B;
      cin >> A >> B;
      v[i].emplace_back((B+180)*10000-0.5, (A+90)*10000-0.5);
    }

    vector<int64_t> allhash(2);
    int64_t base = 56789, basen = 1, md = 123454321;
    for (int i = 0; i < N; i++) basen = (basen * base) % md;
    for (int i = 0; i < 2; i++) {
      sort(v[i].begin(), v[i].end());
      int64_t hash = 0;
      for (int j = 0; j < N; j++) {
        hash = (hash * base + (v[i][j].second + 69) *
                              ((v[i][(j+1)%N].first + 3600000 - v[i][j].first) % 3600000 + 666)) % md;
      }
      for (int j = 0; j < N; j++) {
        hash = (hash * base + (1 + md - basen) *
                              (v[i][j].second + 69) % md *
                              ((v[i][(j+1)%N].first + 3600000 - v[i][j].first) % 3600000 + 666)) % md;
        allhash[i] ^= hash;
      }
    }
    cout << (allhash[0] == allhash[1] ? "Same" : "Different") << endl;
  }
}
