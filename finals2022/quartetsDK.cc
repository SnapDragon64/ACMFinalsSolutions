#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<vector<int>> f(69, vector<int>(69)), cap(69, vector<int>(69));  // Nice^4!
    auto add_edge = [&](int x, int y, int c) { cap[x][y] = c; };
    for (int i = 0; i < 32; i++) add_edge(0, i+1, 1);  // One of each card.
    for (int p = 0; p < 4; p++)
    for (int i = 0; i < 32; i++) {
      add_edge(i+1, p*8 + i/4 + 33, 1);  // Each card goes to set S of player P.
      add_edge(p*8 + i/4 + 33, p + 65, 4);  // Allows restricting count of set S owned by player P.
      add_edge(p + 65, 0, 8);  // Each player P has 8 cards.
    }

    int move, failed = 0;
    auto increase_min = [&](int x, int y, int minf) {
      if (failed || cap[y][x] <= -minf) return;
      vector<int> seen;
      function<bool(int, int)> dfs = [&](int x, int y) {
        if (x == y) return true;
        if (seen[x]) return false;
        seen[x] = true;
        for (int z = 0; z < 69; z++) if (f[x][z] < cap[x][z]) {
          if (dfs(z, y)) {
            f[x][z]++; f[z][x]--;
            return true;
          }
        }
        return false;
      };
      cap[y][x] = -minf;
      while (f[y][x] > cap[y][x]) {
        f[x][y]++; f[y][x]--;
        seen = vector<int>(69);
        if (f[x][y] > cap[x][y] || !dfs(y, x)) { failed = move; return; }
      }
    };
    for (int i = 0; i < 32; i++) increase_min(0, i+1, 1);  // Force all 32 cards to be dealt.

    vector<int> cur_pos(32, -1);
    for (move = 1; move <= N; move++) {
      int X, Y, S;
      char QA, I;
      string YesNo;
      cin >> X >> QA; X--;
      if (QA == 'Q') {
        cin >> S; S--;
        if (failed) continue;
        for (int c = S*4; c < S*4+4; c++) {
          if (cur_pos[c] == -1) increase_min(c+1, X*8 + S + 33, 1);  // Player X must start with this card.
          assert(cur_pos[c] == -1 || cur_pos[c] == X);
          cur_pos[c] = -2;  // Inaccessible for rest of game.
        }
      } else {
        cin >> Y >> S >> I >> YesNo; Y--; S--;
        if (failed) continue;
        bool has = false;
        for (int c = S*4; c < S*4+4; c++) if (cur_pos[c] == X) has = true;
        if (!has) {
          // Player X must start with a new card from S that we don't already know about.
          int nc = 0;
          for (int c = S*4; c < S*4+4; c++) if (cap[X*8 + S + 33][c+1] == -1) nc++;
          increase_min(X*8 + S + 33, X + 65, nc+1);
        }
        int c = S*4 + I-'A';
        if (YesNo == "yes") {
          assert(cur_pos[c] == -1 || cur_pos[c] == Y);
          if (cur_pos[c] == -1) increase_min(c+1, Y*8 + S + 33, 1);  // Player Y must start with this card.
          cur_pos[c] = X;
        } else {
          if (cur_pos[c] == Y) {
            failed = move;
          } else if (cur_pos[c] == -1) {
            increase_min(Y*8 + S + 33, c+1, 0);  // Player Y must not start with this card.
          }
        }
      }
    }

    if (failed) cout << "no" << endl << failed << endl; else cout << "yes" << endl;
  }
}
