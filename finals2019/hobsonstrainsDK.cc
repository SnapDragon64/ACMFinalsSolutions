#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, K;
  while (cin >> N >> K) {
    vector<int> succ(N), ret(N, -1);
    vector<vector<int>> pred(N);
    for (int i = 0; i < N; i++) {
      cin >> succ[i];
      succ[i]--;
      pred[succ[i]].push_back(i);
    }
    for (int x = 0; x < N; x++) if (ret[x] == -1) {
      int c, c2;
      for (c = x, c2 = succ[x]; c != c2 && c != succ[c2]; c = succ[c], c2 = succ[succ[c2]])
        ;
      vector<int> cyc(1, c);
      for (int y = succ[c]; y != c; y = succ[y]) cyc.push_back(y);
      vector<int> diff(cyc.size());
      int base = min(K+1, int(cyc.size()));
      for (int i = 0; i < cyc.size(); i++) {
        for (auto y : pred[cyc[i]]) if (y != cyc[(i+cyc.size()-1)%cyc.size()]) {
          int ns = 0;
          vector<int> stack(1);
          function<void(int,int,int)> doit = [&] (int nd, int prev, int d) {
            int curns = ns;
            if (stack.size() == d) stack.push_back(0);
            ns++; stack[d]++;
            for (auto nd2 : pred[nd]) if (nd2 != prev) doit(nd2, nd, d+1);
            ret[nd] = ns-curns;
            if (stack.size() == d+K+1) { ns -= stack.back(); stack.pop_back(); }
          };
          doit(y, cyc[i], 1);
          for (int j = 1; j < stack.size(); j++) {
            if (K-j+1 >= cyc.size()) {
              base += stack[j];
            } else {
              diff[i] += stack[j];
              diff[(i+K-j+1)%cyc.size()] -= stack[j];
              if (i+K-j+1 >= cyc.size()) base += stack[j];
            }
          }
        }
      }
      for (int i = 0; i < cyc.size(); i++) {
        base += diff[i];
        ret[cyc[i]] = base;
      }
    }
    for (auto n : ret) cout << n << endl;
  }
}
