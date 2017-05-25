#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<pair<pair<int, int>, int>> ul(N), br(N);
    for (int i = 0; i < N; i++) {
      cin >> ul[i].first.first >> ul[i].first.second;
      ul[i].second = i+1;
    }
    for (int i = 0; i < N; i++) {
      cin >> br[i].first.first >> br[i].first.second;
      br[i].second = i+1;
    }
    sort(ul.begin(), ul.end());
    sort(br.begin(), br.end());

    vector<int> ret(N+1);
    map<int, int> ulm;
    vector<pair<pair<int, int>, int>> rows;
    vector<pair<int, pair<int, int>>> cols;
    multiset<int> rowy;
    priority_queue<pair<int, int>> rowexpire;
    ulm[-2e9] = -1;
    for (int i = 0, j = 0; i < N || j < N; ) {
      if (j == N || i < N && ul[i].first.first <= br[j].first.first) {
        ulm[ul[i].first.second] = i;
        i++;
      } else {
        int x2 = br[j].first.first, y2 = br[j].first.second;
        auto it = --ulm.upper_bound(y2);
        int k = it->second, x = ul[k].first.first, y = ul[k].first.second;
        if (k == -1) goto fail;
        ulm.erase(it);
        ret[ul[k].second] = br[j].second;
        rows.push_back(make_pair(make_pair(x, x2), y));
        rows.push_back(make_pair(make_pair(x, x2), y2));
        cols.push_back(make_pair(x, make_pair(y, y2)));
        cols.push_back(make_pair(x2, make_pair(y, y2)));
        j++;
      }
    }

    sort(rows.begin(), rows.end());
    sort(cols.begin(), cols.end());
    for (int i = 0, j = 0; i < 2*N || j < 2*N; ) {
      if (j == 2*N || i < 2*N && rows[i].first.first <= cols[j].first) {
        rowy.insert(rows[i].second);
        rowexpire.push(make_pair(-rows[i].first.second, rows[i].second));
        i++;
      } else {
        while (!rowexpire.empty() && -rowexpire.top().first < cols[j].first) {
          rowy.erase(rowy.lower_bound(rowexpire.top().second));
          rowexpire.pop();
        }
        auto it = rowy.lower_bound(cols[j].second.first);
        auto it2 = rowy.upper_bound(cols[j].second.second);
        if ((++(++it)) != it2) goto fail;
        j++;
      }
    }

    for (int i = 1; i <= N; i++) cout << ret[i] << endl;
    continue;

fail:
    cout << "syntax error" << endl;
  }
}
