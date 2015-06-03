#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

main() {
  int X, Y, i, j, k, x, y, w, h, dx, dy;
  vector<int> vx, vy, vx2, vy2;
  cin >> X >> Y;

  string cmd;
  for (int ncmd = 1; cin >> cmd; ncmd++) {
    if (cmd == "OPEN") {
      cin >> x >> y >> w >> h;
      bool fail = (x+w > X || y+h > Y);
      for (i = 0; i < vx.size(); i++) {
        if (max(vx[i], x) <= min(vx2[i], x+w-1) &&
            max(vy[i], y) <= min(vy2[i], y+h-1)) {
          fail = true;
        }
      }
      if (fail) {
        cout << "Command " << ncmd << ": OPEN - " <<
                "window does not fit" << endl;
        continue;
      }
      vx.push_back(x);
      vy.push_back(y);
      vx2.push_back(x+w-1);
      vy2.push_back(y+h-1);
      assert(vx.size() <= 256);
    } else if (cmd == "RESIZE") {
      cin >> x >> y >> w >> h;
      for (i = 0; i < vx.size(); i++) {
        if (x >= vx[i] && x <= vx2[i] && y >= vy[i] && y <= vy2[i]) break;
      }
      if (i == vx.size()) {
        cout << "Command " << ncmd << ": RESIZE - " <<
                "no window at given position" << endl;
        continue;
      }
      x = vx[i]; y = vy[i];
      bool fail = (x+w > X || y+h > Y);
      for (j = 0; j < vx.size(); j++) if (j != i) {
        if (max(vx[j], x) <= min(vx2[j], x+w-1) &&
            max(vy[j], y) <= min(vy2[j], y+h-1)) {
          fail = true;
        }
      }
      if (fail) {
        cout << "Command " << ncmd << ": RESIZE - " <<
                "window does not fit" << endl;
        continue;
      }
      vx2[i] = x+w-1;
      vy2[i] = y+h-1;
    } else if (cmd == "CLOSE") {
      cin >> x >> y;
      for (i = 0; i < vx.size(); i++) {
        if (x >= vx[i] && x <= vx2[i] && y >= vy[i] && y <= vy2[i]) break;
      }
      if (i == vx.size()) {
        cout << "Command " << ncmd << ": CLOSE - " <<
                "no window at given position" << endl;
        continue;
      }
      vx.erase(vx.begin()+i);
      vy.erase(vy.begin()+i);
      vx2.erase(vx2.begin()+i);
      vy2.erase(vy2.begin()+i);
    } else if (cmd == "MOVE") {
      cin >> x >> y >> dx >> dy;
      for (i = 0; i < vx.size(); i++) {
        if (x >= vx[i] && x <= vx2[i] && y >= vy[i] && y <= vy2[i]) break;
      }
      if (i == vx.size()) {
        cout << "Command " << ncmd << ": MOVE - " <<
                "no window at given position" << endl;
        continue;
      }

      bool xmove = (dx != 0);
      bool flip = (dx < 0 || dy < 0);
      if (!xmove) {
        vx.swap(vy); vx2.swap(vy2); swap(X, Y); swap(dx, dy);
      }
      if (flip) {
        for (j = 0; j < vx.size(); j++) {
          swap(vx[j], vx2[j]);
          vx[j] = X-1-vx[j];
          vx2[j] = X-1-vx2[j];
        }
        dx = -dx;
      }

      int odx = dx;
      vector<int> u(vx.size());
      u[i] = 1;
      while (dx > 0) {
        int gap = dx, gapk = -1;
        for (j = 0; j < vx.size(); j++) if (u[j])
          gap = min(gap, X-1-vx2[j]);
        if (gap == 0) break;
        for (j = 0; j < vx.size(); j++) if (u[j])
        for (k = 0; k < vx.size(); k++) if (!u[k]) {
          if (max(vy[j], vy[k]) <= min(vy2[j], vy2[k]) &&
              vx[k] > vx2[j] && vx[k]-vx2[j]-1 < gap) {
            gap = vx[k]-vx2[j]-1;
            gapk = k;
          }
        }
        for (j = 0; j < vx.size(); j++) if (u[j]) {
          vx[j] += gap;
          vx2[j] += gap;
        }
        dx -= gap;
        if (gapk != -1) u[gapk] = 1;
      }
      if (dx) {
        cout << "Command " << ncmd << ": MOVE - " <<
                "moved " << odx-dx << " instead of " << odx << endl;
      }

      if (flip) {
        for (j = 0; j < vx.size(); j++) {
          swap(vx[j], vx2[j]);
          vx[j] = X-1-vx[j];
          vx2[j] = X-1-vx2[j];
        }
      }
      if (!xmove) {
        vx.swap(vy); vx2.swap(vy2); swap(X, Y);
      }
    }
/*cout << vx.size() << " window(s):" << endl;
for (int i = 0; i < vx.size(); i++) {
  cout << vx[i] << ' ' << vy[i] << ' ' << vx2[i]-vx[i]+1 << ' ' << vy2[i]-vy[i]+1 << endl;
}*/
  }

  cout << vx.size() << " window(s):" << endl;
  for (int i = 0; i < vx.size(); i++) {
    cout << vx[i] << ' ' << vy[i] << ' ' << vx2[i]-vx[i]+1 << ' ' << vy2[i]-vy[i]+1 << endl;
  }
}
