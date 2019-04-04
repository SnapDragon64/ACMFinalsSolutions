#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int64_t N, H, HC, DC;
  while (cin >> N >> H >> HC >> DC) {
    vector<int64_t> X(N), Y(N);
    for (int i = 0; i < N; i++) cin >> X[i] >> Y[i];
    vector<int64_t> cost(N, 1e18);
    cost[0] = HC*(H-Y[0]);
    for (int i = 0; i < N; i++) {
      int64_t mind = 0, maxd = 2*(H-Y[i]);
      for (int j = i+1; j < N; j++) {
        int64_t d = X[j]-X[i], h = H-Y[j];
        if (d > maxd) break;
        if (2*h <= d) mind = max(mind, 2*d+2*h-int64_t(sqrt(8*d*h)));
        maxd = min(maxd, 2*d+2*h+int64_t(sqrt(8*d*h)));
        if (mind > maxd) break;
        if (d >= mind && d <= maxd) {
          cost[j] = min(cost[j], cost[i] + HC*h + DC*d*d);
        }
      }
    }
    if (cost[N-1] == 1e18) cout << "impossible" << endl; else cout << cost[N-1] << endl;
  }
}
