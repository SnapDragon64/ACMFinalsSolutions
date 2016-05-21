#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

vector<int> dijkstra(const vector<vector<int>>& e,
                     const vector<vector<int>>& el,
                     int s) {
  vector<int> dist(e.size(), 1e9);
  priority_queue<pair<int, int>> q;
  q.push(make_pair(0, s));
  while (!q.empty()) {
    int d = -q.top().first, x = q.top().second;
    q.pop();
    if (d >= dist[x]) continue;
    dist[x] = d;
    for (int i = 0; i < e[x].size(); i++) {
      q.push(make_pair(-d-el[x][i], e[x][i]));
    }
  }
  return dist;
}

int main() {
  int N, B, S, R;
  while (cin >> N >> B >> S >> R) {
    vector<vector<int> > fe(N), fel(N), be(N), bel(N);
    for (int i = 0; i < R; i++) {
      int X, Y, L;
      cin >> X >> Y >> L;
      X--; Y--;
      fe[X].push_back(Y);
      fel[X].push_back(L);
      be[Y].push_back(X);
      bel[Y].push_back(L);
    }

    vector<int> fdist = dijkstra(fe, fel, B);
    vector<int> bdist = dijkstra(be, bel, B);
    vector<int> dist(B);
    for (int i = 0; i < B; i++) dist[i] = fdist[i] + bdist[i];
    sort(dist.begin(), dist.end());
    vector<long long> cumdist(B+1);
    for (int i = 0; i < B; i++) cumdist[i+1] = cumdist[i] + dist[i];

    vector<long long> dyn(B+1);
    for (int i = 1; i <= B; i++) dyn[i] = (i-1)*cumdist[i];
    for (int s = 2; s <= S; s++) {
      vector<long long> dyn2 = dyn;
      for (int i = 1; i <= B; i++)
      for (int j = 1; j*s <= i; j++) {
        dyn2[i] = min(dyn2[i], dyn[i-j] + (j-1)*(cumdist[i]-cumdist[i-j]));
      }
      dyn.swap(dyn2);
    }

    cout << dyn[B] << endl;
  }
}
