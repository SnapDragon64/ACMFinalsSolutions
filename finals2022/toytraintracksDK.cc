#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

int main() {
  int S, C;
  while (cin >> S >> C) {
    string s = "L", mid = "LL", e = "L";
    C -= 4;
    if (C >= 8) {
      s += "RLLR";
      for (C -= 8; C >= 4; C -= 4) { s += "LR"; e += "RL"; }
      e += "RLLR";
    }
    if (S >= 2) {
      if (C >= 6) { mid = "RLLSRLLSLR"; C -= 6; S -= 2; }
      if (C >= 4) { s += "RLLR"; e += "SS"; C -= 4; S -= 2; }
      if (C >= 2) { mid = "SLSLLR"; C -= 2; S -= 2; }
    }
    while (S >= 2) { s += "S"; e += "S"; S -= 2; }
    reverse(e.begin(), e.end());
    cout << s << mid << e << endl;
  }
}
