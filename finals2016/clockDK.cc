#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef vector<string> VS;

char digit[7][10][5] = {
{".XX.","....",".XX.",".XX.","....",".XX.",".XX.",".XX.",".XX.",".XX."},
{"X..X","...X","...X","...X","X..X","X...","X...","...X","X..X","X..X"},
{"X..X","...X","...X","...X","X..X","X...","X...","...X","X..X","X..X"},
{"....","....",".XX.",".XX.",".XX.",".XX.",".XX.","....",".XX.",".XX."},
{"X..X","...X","X...","...X","...X","...X","X..X","...X","X..X","...X"},
{"X..X","...X","X...","...X","...X","...X","X..X","...X","X..X","...X"},
{".XX.","....",".XX.",".XX.","....",".XX.",".XX.","....",".XX.",".XX."}
};

char display[7][22] = {
".??...??.....??...??.",
"?..?.?..?...?..?.?..?",
"?..?.?..?.?.?..?.?..?",
".??...??.....??...??.",
"?..?.?..?.?.?..?.?..?",
"?..?.?..?...?..?.?..?",
".??...??.....??...??."
};

void WriteDigit(int x, int dig, VS* display) {
  for (int y = 0; y < 7; y++)
  for (int i = 0; i < 4; i++)
    (*display)[y][x+i] = digit[y][dig][i];
}

VS GetDisplay(int time) {
  VS ret(7, string(21, '.'));
  ret[2][10] = ret[4][10] = 'X';
  if (time >= 10*60) WriteDigit(0, time/10/60, &ret);
  WriteDigit(5, (time/60)%10, &ret);
  WriteDigit(12, ((time%60)/10)%10, &ret);
  WriteDigit(17, time%10, &ret);
  return ret;
}

main() {
  int N;
  while (cin >> N && N) {
    vector<VS> d(N, VS(7));
    for (int i = 0; i < N; i++)
    for (int y = 0; y < 7; y++)
      cin >> d[i][y];

    VS ret(7);
    for (int y = 0; y < 7; y++) ret[y] = display[y];

    for (int i = 1; i < N; i++)
    for (int y = 0; y < 7; y++)
    for (int x = 0; x < 21; x++)
      if (d[i-1][y][x] != d[i][y][x])
        ret[y][x] = 'W';
    for (int y = 0; y < 7; y++)
    for (int x = 0; x < 21; x++)
      if (ret[y][x] == '?')
        ret[y][x] = (d[0][y][x] == 'X') ? '1' : '0';

    bool valid = false;
    for (int stime = 0; stime < 24*60; stime++) {
      vector<VS> displays(N);
      for (int i = 0; i < N; i++) {
        displays[i] = GetDisplay((stime + i) % (24*60));
        for (int y = 0; y < 7; y++)
        for (int x = 0; x < 21; x++)
          if (ret[y][x] == 'W' && displays[i][y][x] != d[i][y][x]) {
            goto fail;
          }
      }

      valid = true;
      for (int y = 0; y < 7; y++)
      for (int x = 0; x < 21; x++)
        if (ret[y][x] == '0' || ret[y][x] == '1') {
          int i;
          for (i = 0; i < N; i++) {
            if (ret[y][x] == '0' && displays[i][y][x] == 'X') break;
            if (ret[y][x] == '1' && displays[i][y][x] == '.') break;
          }
          if (i == N) ret[y][x] = '?';
        }

fail:;
    }

    if (!valid) {
      cout << "impossible" << endl;
    } else {
      for (int y = 0; y < 7; y++) cout << ret[y] << endl;
    }
  }
}
