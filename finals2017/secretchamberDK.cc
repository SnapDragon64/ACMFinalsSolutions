#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

int main() {
  int M, N;
  while (cin >> M >> N) {
    VVI v(26, VI(26));
    for (int i = 0; i < 26; i++) v[i][i] = 1;
    for (int i = 0; i < M; i++) {
      char ch1, ch2;
      cin >> ch1 >> ch2;
      v[ch1-'a'][ch2-'a'] = 1;
    }

    for (int k = 0; k < 26; k++)
    for (int i = 0; i < 26; i++)
    for (int j = 0; j < 26; j++)
      v[i][j] = max(v[i][j], min(v[i][k], v[k][j]));

    for (int i = 0; i < N; i++) {
      string w1, w2;
      cin >> w1 >> w2;
      bool equal = (w1.size() == w2.size());
      for (int i = 0; equal && i < w1.size(); i++) {
        if (!v[w1[i]-'a'][w2[i]-'a']) equal = false;
      }
      cout << (equal ? "yes" : "no") << endl;
    }
  }
}
