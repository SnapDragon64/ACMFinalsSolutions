#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;

int g[301][301], mncol[301][301], szmx[301][301];

main() {
  int A, B, X, Y;
  while (cin >> A >> B >> Y >> X) {
    for (int y = 0; y < Y; y++)
    for (int x = 0; x < X; x++)
      cin >> g[y][x];

    memset(szmx, 0, sizeof(szmx));
    for (int y1 = 0; y1 < Y; y1++) {
      for (int x = 0; x < X; x++)
        mncol[y1][x] = g[y1][x];
      for (int y2 = y1+1; y2 < Y; y2++)
      for (int x = 0; x < X; x++)
        mncol[y2][x] = min(mncol[y2-1][x], g[y2][x]);

      for (int x1 = 0; x1 < X; x1++)
      for (int y2 = y1; y2 < Y; y2++) {
        int cur = 1000000001;
        int* mnc = &mncol[y2][x1];
        int* sm = &szmx[y2-y1+1][1];
        for (int x2 = x1; x2 < X; x2++) {
          cur = min(cur, *mnc);
          *sm = max(*sm, cur);
          mnc++; sm++;
        }
      }
    }

    long long ret = 0;
    for (int a = 1; a <= A; a++)
    for (int b = 1; b <= B; b++) {
      //long long depth = max(szmx[a][b], szmx[b][a]);
      long long depth = szmx[a][b];
      if (depth == 0) continue;
      ret = max(ret, a*b * (depth + (a*b*depth-1) / (X*Y-a*b)));
    }
    cout << ret << endl;
  }
}
