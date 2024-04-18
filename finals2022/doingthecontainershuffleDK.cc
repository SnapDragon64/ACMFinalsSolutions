#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

void fenwickAdd(vector<int>& f, int x, int v) {
  for (int b = 1; x < f.size(); x |= b, b <<= 1) if (!(x&b)) f[x] += v;
}

int fenwickCount(vector<int>& f, int s, int e) {
  int ret = 0;
  for (int b = 1; e > 0; e &= ~b, b <<= 1) if (e&b) ret += f[e-1];
  for (int b = 1; s > 0; s &= ~b, b <<= 1) if (s&b) ret -= f[s-1];
  return ret;
}

int main() {
  int N;
  while (cin >> N) {
    vector<int> f(N+2);
    for (int x = 1; x <= N; x++) fenwickAdd(f, x, 1);
    int64_t ret = 0;
    for (int i = 0, last = N+1; i < N; i++) {
      int cur;
      cin >> cur;
      ret += 2;
      fenwickAdd(f, cur, -1);
      ret += fenwickCount(f, min(last, cur), N+2);
      last = cur;
    }
    cout << fixed << setprecision(3) << ret/2.0 << endl;
  }
}
