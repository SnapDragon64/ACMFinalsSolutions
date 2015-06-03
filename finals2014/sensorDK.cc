#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

int N, D;
int dist[101][101];
vector<int> v1, v2, v1seen, v2seen, v1forward, v2back, cur;

bool doit(int x) {
  if (v1seen[x]) return false;
  v1seen[x] = true;
  for (int y = 0; y < v2.size(); y++) if (dist[v1[x]][v2[y]] > D*D) {
    if (v2back[y] == -1 || doit(v2back[y])) {
      v1forward[x] = y;
      v2back[y] = x;
      return true;
    }
  }
  return false;
}

void mark(int y) {
  if (v2seen[y]) return;
  v2seen[y] = true;
  cur.push_back(v2[y]);
  int x = v2back[y];
  if (x == -1) return;
  v1seen[x] = true;
  for (int x2 = 0; x2 < v1.size(); x2++)
    if (dist[v1[x2]][v2[y]] > D*D) mark(v1forward[x2]);
}

main() {
  while (cin >> N >> D) {
    vector<int> vx(N), vy(N);
    for (int i = 0; i < N; i++) cin >> vx[i] >> vy[i];
    for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      dist[i][j] = (vx[i]-vx[j])*(vx[i]-vx[j]) + (vy[i]-vy[j])*(vy[i]-vy[j]);

    vector<int> ret(1, 0);
    for (int i = 0; i < N; i++)
    for (int j = i+1; j < N; j++) if (dist[i][j] <= D*D) {
      v1.clear(); v2.clear();
      for (int k = 0; k < N; k++) {
        if (k != i && k != j && dist[i][k] <= dist[i][j]
                             && dist[j][k] <= dist[i][j]) {
          if ((vx[j]-vx[i])*(vy[k]-vy[i]) - (vy[j]-vy[i])*(vx[k]-vx[i]) <= 0) {
            v1.push_back(k);
          } else {
            v2.push_back(k);
          }
        }
      }

      v1forward = vector<int>(v1.size(), -1);
      v2back = vector<int>(v2.size(), -1);
      for (int k = 0; k < v1.size(); k++) {
        v1seen = vector<int>(v1.size(), 0);
        doit(k);
      }

      cur.clear();
      cur.push_back(i);
      cur.push_back(j);
      v1seen = vector<int>(v1.size(), 0);
      v2seen = vector<int>(v2.size(), 0);
      for (int x = 0; x < v1.size(); x++) if (!v1seen[x] && v1forward[x] != -1)
      for (int y = 0; y < v2.size(); y++)
      if (v2back[y] == -1 && dist[v1[x]][v2[y]] > D*D) {
        mark(v1forward[x]);
      }
      for (int x = 0; x < v1.size(); x++) if (!v1seen[x]) {
        cur.push_back(v1[x]);
        if (v1forward[x] != -1) v2seen[v1forward[x]] = true;
      }
      for (int y = 0; y < v2.size(); y++) if (!v2seen[y]) {
        cur.push_back(v2[y]);
      }

      if (cur.size() > ret.size()) ret = cur;
    }

    cout << ret.size() << endl;
    for (int i = 0; i < ret.size(); i++) {
      if (i) cout << ' ';
      cout << ret[i]+1;
    }
    cout << endl;
  }
}
