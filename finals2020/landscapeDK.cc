#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, K;
  while (cin >> N >> K) {
    vector<int> xd(N+10);
    for (int i = 0; i < K; i++) {
      char ch;
      int X1, X2;
      cin >> ch >> X1 >> X2;
      if (ch == 'R' || ch == 'D') {
        xd[X1-1] += (ch == 'R' ? 1 : -1);
        xd[X1]   -= (ch == 'R' ? 1 : -1);
        xd[X2]   -= (ch == 'R' ? 1 : -1);
        xd[X2+1] += (ch == 'R' ? 1 : -1);
      } else {
        xd[X1-1]        += (ch == 'H' ? 1 : -1);
        xd[(X2+X1  )/2] -= (ch == 'H' ? 1 : -1);
        xd[(X2+X1+1)/2] -= (ch == 'H' ? 1 : -1);
        xd[X2+1]          += (ch == 'H' ? 1 : -1);
      }
    }
    int64_t x = 0, d = 0;
    for (int i = 0; i < xd.size(); i++) {
      x += d;
      d += xd[i];
      if (i >= 1 && i <= N) cout << x << endl;
    }
  }
}
