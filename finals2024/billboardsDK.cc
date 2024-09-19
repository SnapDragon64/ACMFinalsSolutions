#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

int main() {
  int N, M, L, A, B;
  while (cin >> N >> L) {
    vector<vector<tuple<double, double, double>>> areas(N);
    for (int i = 0; i < N; i++) {
      cin >> M;
      double ar = 0.0, lasta = 0.0, lastb = 0.0;
      for (int j = 0; j < M; j++) {
        cin >> A >> B;
        ar += (A-lasta)*(B+lastb)/2;
        lasta = A; lastb = B;
        areas[i].push_back({ar, lasta, lastb});
      }
    }

    double curx = 0.0;
    vector<int> curj(N, 0), goalj(N, 0);
    for (int n = 0; n < N-1; n++) {
      double minx = L+1;
      int mini = -1;
      for (int i = 0; i < N; i++) if (curj[i] != -1) {
        while (get<1>(areas[i][curj[i]+1]) < curx) curj[i]++;
        auto [a1, x1, y1] = areas[i][curj[i]];
        auto [a2, x2, y2] = areas[i][curj[i]+1];
        double y = y1 + (y2-y1) * (curx-x1) / (x2-x1);
        double cura = a1 + (curx-x1)*(y1+y)/2;
        double goala = cura + get<0>(areas[i].back()) / N;
        while (get<0>(areas[i][goalj[i]+1]) < goala) goalj[i]++;
        auto [a3, x3, y3] = areas[i][goalj[i]];
        auto [a4, x4, y4] = areas[i][goalj[i]+1];
        // 2ad = v_f^2 - v_i^2
        double acc = (y4-y3)/(x4-x3);
        double goalx = x3 + (x4-x3) * (acc == 0.0 ? (goala-a3)/(a4-a3) : (sqrt(2*acc*(goala-a3)+y3*y3)-y3)/(y4-y3));
        if (goalx < minx) { minx = goalx; mini = i; }
      }
      curx = minx;
      curj[mini] = -1;
      cout << fixed << setprecision(10) << minx << ' ' << mini+1 << endl;
    }
    for (int i = 0; i < N; i++) if (curj[i] != -1) cout << L << ' ' << i+1 << endl;
  }
}
