#include <cassert>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>
using namespace std;

int main() {
  int N, S, L, R;
  while (cin >> N >> S) {
    priority_queue<tuple<int,bool,int,int,int>> events;
    for (int i = 1; i <= N; i++) {
      cin >> L >> R;
      L = 2*L-S; R = 2*R-S;
      events.push({ R, true,  i,  L, true });
      events.push({ L, false, i,  0, true });
      events.push({-L, true,  i, -R, false});
      events.push({-R, false, i,  0, false});
    }

    vector<bool> paired(N+1);
    vector<pair<int,int>> ret;
    priority_queue<pair<int,int>> exp[2];
    while (!events.empty()) {
      auto [t,add,n,et,gp] = events.top(); events.pop();
      if (add) {
        exp[gp].push({et,n});
      } else {
        if (paired[n]) continue;
        while (!exp[!gp].empty()) {
          auto [t2,n2] = exp[!gp].top(); exp[!gp].pop();
          if (t2 <= t && n2 != n && !paired[n2]) {
            paired[n] = paired[n2] = true;
            ret.push_back({n, n2});
            break;
          }
        }
      }
    }

    cout << ret.size() << endl;
    for (auto [x,y] : ret) cout << x << ' ' << y << endl;
  }
}
