#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Partial {
  int t, curi, nphoto, dli;
  long long sumstarts;
  bool after_photo;

  // Between two states at the same time we always maximize # of photos taken,
  // and the times we started those photos.
  bool operator<(const Partial& p) const {
    if (t != p.t) return t > p.t;
    if (nphoto != p.nphoto) return nphoto < p.nphoto;
    return sumstarts < p.sumstarts;
  }
};

int main() {
  int N, T;
  while (cin >> N >> T) {
    vector<pair<int, int>> V(N);
    for (int i = 0; i < N; i++) cin >> V[i].first >> V[i].second;
    sort(V.begin(), V.end());

    bool ret = false;
    priority_queue<Partial> q;
    vector<priority_queue<int>> deadlines(1);  // Maintain O(N) heaps.
    Partial start;
    start.after_photo = false;
    start.t = start.curi = start.nphoto = start.dli = start.sumstarts = 0;
    q.push(start);

    while (!q.empty()) {
      Partial p = q.top();
      while (!q.empty() && q.top().t == p.t) q.pop();
      if (p.nphoto == N) {
        ret = true;
        break;
      }

      if (p.after_photo) {
        deadlines[p.dli].pop();
      } else {
        deadlines.push_back(deadlines[p.dli]);
        p.dli = deadlines.size()-1;
      }
      priority_queue<int>& dl = deadlines[p.dli];
      for (; p.curi < V.size() && V[p.curi].first <= p.t; p.curi++) {
        dl.push(-V[p.curi].second);
      }
      if (dl.size() && -dl.top() < p.t + T) continue;

      if (p.curi < V.size() && (!dl.size() || V[p.curi].first < p.t+T)) {
        p.after_photo = false;
        int ot = p.t;
        p.t = V[p.curi].first;
        q.push(p);
        p.t = ot;
      }

      if (dl.size()) {
        p.after_photo = true;
        p.nphoto++;
        p.sumstarts += p.t;
        p.t += T;
        q.push(p);
      }
    }

    cout << (ret ? "yes" : "no") << endl;
  }
}
