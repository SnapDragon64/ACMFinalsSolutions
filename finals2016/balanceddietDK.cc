#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
  int M, K;
  while (cin >> M >> K) {
    vector<int> f(M);
    for (int i = 0; i < M; i++) cin >> f[i];
    long long ftot = 0;
    for (int i = 0; i < M; i++) ftot += f[i];
    vector<int> atot(M);
    for (int i = 0, x; i < K; i++) {
      cin >> x;
      atot[x-1]++;
    }

    priority_queue<pair<int, int> > q;
    for (int i = 0; i < M; i++) {
      q.push(make_pair(-((atot[i]+1)*ftot+f[i]-1)/f[i], i));
    }
    int n;
    for (n = K+1; n <= K+2*ftot; n++) {
//cerr << n << ' ' << -q.top().first << ' ' << q.top().second << endl;
      if (-q.top().first < n) {n = -q.top().first; break;}
      int i = q.top().second;
      atot[i]++;
      q.pop();
      q.push(make_pair(-((atot[i]+1)*ftot+f[i]-1)/f[i], i));
    }

    if (n > K+2*ftot) {
      cout << "forever" << endl;
    } else {
      cout << n-K-1 << endl;
    }
  }
}
