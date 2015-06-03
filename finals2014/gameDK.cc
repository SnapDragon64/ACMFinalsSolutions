#include <iostream>
#include <string>
#include <vector>
using namespace std;

main() {
  int N;
  while (cin >> N) {
    vector<vector<int> > v(N);
    for (int i = 0; i < N; i++) {
      int M;
      cin >> M;
      for (int j = 0; j < M; j++) {
        string s;
        cin >> s;
        int b = 0;
        for (int k = 0; k < s.size(); k++) b |= (1<<(s[k]-'a'));
        v[i].push_back(b);
      }
    }

    vector<vector<int> > ret(N, vector<int>(N, -1));
    for (int es = 0; es < N; es++) {
      int b = (1<<es);
      ret[es][es] = 0;
      for (int steps = 1; ; steps++) {
        int b2 = b;
        for (int i = 0; i < N; i++) if (!(b&(1<<i))) {
          for (int j = 0; j < v[i].size(); j++)
            if ((b & v[i][j]) == v[i][j]) {
              b2 |= (1<<i);
              ret[i][es] = steps;
              break;
            }
        }

        if (b2 == b) break;
        b = b2;
      }
    }

    for (int ss = 0; ss < N; ss++) {
      for (int es = 0; es < N; es++) {
        cout << ret[ss][es];
        if (es < N-1) cout << ' '; else cout << endl;
      }
    }
  }
}
