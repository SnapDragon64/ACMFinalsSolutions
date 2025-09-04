#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int X, Y;
  while (cin >> Y >> X) {
    vector<string> G(Y);
    string S;
    for (auto& s : G) cin >> s;
    cin >> S;

    string order = "ENSW";
    vector<string> orders;
    do { orders.push_back(order); } while (next_permutation(order.begin(), order.end()));
    int ret = 0, bo = (1<<24)-1, bx, by;
    for (int y = 0; y < Y; y++) for (int x = 0; x < X; x++) if (G[y][x] == 'S') { bx = x; by = y; }

    for (auto ch : S) {
      int valid = 0;
      for (int i = 0; i < 24; i++) {
        for (char move : orders[i]) {
          int bx2 = bx + (move == 'E') - (move == 'W');
          int by2 = by + (move == 'S') - (move == 'N');
          if (bx2 >= 0 && bx2 < X && by2 >= 0 && by2 < Y && G[by2][bx2] != '#') {
            if (move == ch) valid |= (1<<i);
            break;
          }
        }
      }
      bo &= valid;
      if (bo == 0) {
        ret++;
        bo = valid;
      }
      bx += (ch == 'E') - (ch == 'W');
      by += (ch == 'S') - (ch == 'N');
    }
    cout << ret << endl;
  }
}
