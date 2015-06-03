#include <iostream>
#include <vector>
using namespace std;

#define EPS 1e-9

main() {
  int N;
  while (cin >> N) {
    vector<int> vx(N), vy(N);
    int mnx = 1000000000, mxx = -1000000000;
    for (int i = 0; i < N; i++) {
      cin >> vx[i] >> vy[i];
      if (vy[i] == 0) {mnx = min(mnx, vx[i]); mxx = max(mxx, vx[i]);}
    }
    vx.push_back(vx[0]); vy.push_back(vy[0]);

    double tcx = 0, tar = 0;
    for (int i = 0; i < N; i++) {
      double ar = (vx[i]*vy[i+1] - vy[i]*vx[i+1]) / 2.0;
      double cx = (vx[i] + vx[i+1]) / 3.0;
      double cy = (vy[i] + vy[i+1]) / 3.0;
      tcx += cx * ar;
      tar += ar;
    }
    if (tar < 0.0) {tar = -tar; tcx = -tcx;}
    double cx = tcx / tar;

    if (cx < mnx-EPS && vx[0] <= mnx || cx > mxx+EPS && vx[0] >= mxx) {
      cout << "unstable" << endl;
    } else {
      if (cx < mnx-EPS) {
        double a = tar * (mnx-cx) / (vx[0]-mnx);
        cout << (long long)(a+EPS) << " .. ";
      } else if (cx > mxx+EPS) {
        double a = tar * (cx-mxx) / (mxx-vx[0]);
        cout << (long long)(a+EPS) << " .. ";
      } else {
        cout << "0 .. ";
      }
      if (vx[0] >= mnx && vx[0] <= mxx) {
        cout << "inf" << endl;
      } else if (vx[0] > mxx) {
        double b = tar * (mxx-cx) / (vx[0]-mxx);
        cout << (long long)(b+1-EPS) << endl;
      } else {
        double b = tar * (cx-mnx) / (mnx-vx[0]);
        cout << (long long)(b+1-EPS) << endl;
      }
    }
  }
}
