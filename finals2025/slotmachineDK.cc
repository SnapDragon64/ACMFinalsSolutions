#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, k, k2;
  cin >> N >> k;
  for (int i = 1; i <= N; i++) {
    for (;;) {
      cout << i << " " << 1 << endl;
      cin >> k2;
      swap(k, k2);
      if (k == 1) return 0;
      if (k > k2) break;
    }
  }
  assert(k == N);
  vector<int> perm{1}, used(N+1);
  while (perm.size() < N) {
    cout << perm.back() << " " << 1 << endl;
    cin >> k;
    assert(k == N-1);
    for (int i = 1; i <= N; i++) if (i != perm.back()) {
      cout << i << " " << -1 << endl;
      cin >> k2;
      if (k2 == 1) return 0;
      cout << i << " " << 1 << endl;
      cin >> k;
      if (k2 == N) {
        cout << perm.back() << " " << -1 << endl;
        cin >> k;
        perm.push_back(i);
        break;
      }
    }
  }
  for (int i = 0; i < N-1; i++) {
    cout << perm[i] << ' ' << N-1-i << endl;
    cin >> k;
  }
  assert(k == 1);
}
