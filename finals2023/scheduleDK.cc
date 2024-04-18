#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, W;
  while (cin >> N >> W) {
    int c;
    vector<vector<int>> sch;
    for (c = 4; c <= W; c++) {
      sch.clear();
      vector<int> cur(c, 1);
      for (int i = c/2; i < c; i++) cur[i] = 2;
      do {
        sch.push_back(cur);
        next_permutation(cur.begin(), cur.end());
      } while (cur[0] == 1);
      if (sch.size() >= N) break;
    }
    if (c > W) { cout << "infinity" << endl; continue; }
    cout << c << endl;
    for (int i = 0; i < W; i++) {
      for (int j = 0; j < N; j++) cout << sch[j][i%c];
      cout << endl;
    }
  }
}
