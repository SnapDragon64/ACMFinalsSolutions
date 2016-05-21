#include <cstring>
#include <iostream>
#include <string>
using namespace std;

int c[52][52];

main() {
  int N;
  while (cin >> N) {
    memset(c, 0, sizeof(c));
    for (int i = 0; i < N; i++) {
      string s;
      cin >> s;
      for (int i = 0; i < 8; i += 2) if (s[i] != '0')
      for (int j = 0; j < 8; j += 2) if (s[j] != '0' && j != i) {
        c[s[i]-'A' + (s[i+1]=='+'?0:26)][s[j]-'A' + (s[j+1]=='+'?26:0)] = true;
      }
    }
    for (int k = 0; k < 52; k++)
    for (int i = 0; i < 52; i++)
    for (int j = 0; j < 52; j++)
      c[i][j] |= (c[i][k] & c[k][j]);
    bool bounded = true;
    for (int i = 0; i < 52; i++)
      if (c[i][i]) bounded = false;
    if (bounded)
      cout << "bounded" << endl;
    else
      cout << "unbounded" << endl;
  }
}
