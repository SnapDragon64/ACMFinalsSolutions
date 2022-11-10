#include <iostream>
#include <vector>
using namespace std;

int X, Y, N;
vector<vector<char>> g;
vector<int> wm, wn;

void doit(int x, int y) {
  char ch = (x >= 1 && x <= X && y >= 1 && y <= Y ? g[y][x] : '.');
  if (!ch) return;
  for (int i = 0; i < N; i++) {
    int x2 = x + (ch == '.' ? wm[i] : -wm[i]);
    int y2 = y + (ch == '.' ? wn[i] : -wn[i]);
    if (x2 >= 1 && x2 <= X && y2 >= 1 && y2 <= Y && !g[y2][x2]) {
      g[y2][x2] = ch;
      doit(x2, y2);
    }
  }
}

int main() {
  while (cin >> X >> Y >> N) {
    g = vector<vector<char>>(Y+1, vector<char>(X+1));
    wm = wn = vector<int>(N);
    for (int i = 0; i < N; i++) {
      int B, x, y;
      cin >> wm[i] >> wn[i] >> B;
      for (int j = 0; j < B; j++) {
        cin >> x >> y;
        g[y][x] = '#';
        int x2 = x-wm[i], y2 = y-wn[i];
        if (x2 >= 1 && x2 <= X && y2 >= 1 && y2 <= Y) g[y2][x2] = '.';
      }
    }
    for (int y = -Y; y <= 2*Y; y++) for (int x = -X; x <= 2*X; x++) doit(x, y);

    for (int y = 1; y <= Y; y++) {
      for (int x = 1; x <= X; x++) cout << (g[y][x] ? g[y][x] : '.');
      cout << endl;
    }
    cout << endl;
    for (int y = 1; y <= Y; y++) {
      for (int x = 1; x <= X; x++) cout << (g[y][x] ? g[y][x] : '#');
      cout << endl;
    }
  }
}
