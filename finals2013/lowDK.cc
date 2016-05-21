#include <algorithm>
#include <iostream>
using namespace std;

int b[1000001];

main() {
  int n, k;
  while (cin >> n >> k) {
    int N = 2*n*k;
    for (int i = 0; i < N; i++) cin >> b[i];
    sort(b, b+N);

    int lo = 0, hi = 1000000000;
    while (lo < hi) {
      int d = (lo+hi)/2;
      long long junk = 0;
      for (int i = 0; i < N; i++) {
        if (i+1 < N && b[i+1]-b[i] <= d) {
          i++;
          junk += 2*(k-1);
        } else {
          if (--junk < 0) break;
        }
      }
      if (junk < 0) lo = d+1; else hi = d;
    }
    cout << lo << endl;
  }
}
