#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

int main() {
  int N, M;
  while (cin >> N >> M) {
    vector<string> rname(N);
    vector<vector<int>> ce(M+1);
    vector<long long> cmin(M+1, 1e18), rret(N);
    vector<int> rc(N), rd(N), cn(M+1), cc(M+1), cd(M+1);

    priority_queue<pair<long long, int>> event;
    for (int i = 0; i < N; i++) {
      cin >> rname[i] >> rc[i] >> rd[i];
      ce[rc[i]].push_back(~i);
      cn[rc[i]]++;
      event.push(make_pair(-rd[i], rc[i]));
    }
    for (int i = 1; i <= M; i++) {
      cin >> cc[i] >> cd[i];
      ce[cc[i]].push_back(i);
      cn[cc[i]]++;
    }

    vector<pair<long long, int>> rq, q;
    q.push_back(make_pair(0, 0));
    while (!q.empty()) {
      int x = q.back().second;
      long long d = q.back().first;
      q.pop_back();
      for (int i = 0; i < ce[x].size(); i++) {
        if (ce[x][i] < 0) {
          rq.push_back(make_pair(d+rd[~ce[x][i]], ~ce[x][i]));
        } else {
          q.push_back(make_pair(d+cd[ce[x][i]], ce[x][i]));
        }
      }
    }
    sort(rq.begin(), rq.end());

    int smaller = 0;
    for (int i = 0; i < N; i++) {
      while (event.size() && -event.top().first <= rq[i].first) {
        long long d = -event.top().first;
        int x = event.top().second;
        event.pop();
        cmin[x] = min(cmin[x], d);
        cn[x]--;
        if (cn[x] > 0 || x == 0) {
          smaller++;
        } else {
          event.push(make_pair(-cmin[x]-cd[x], cc[x]));
        }
      }
      rret[rq[i].second] = N-smaller+1;
    }

    for (int i = 0; i < N; i++) cout << rname[i] << ' ' << rret[i] << endl;
  }
}
