#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int main() {
  int64_t N, M, A, B;
  while (cin >> N >> M) {
    vector<vector<int64_t>> comp;
    vector<int64_t> compCities, compi(2*N);
    for (int64_t x = 0; x < 2*N; x++) {
      compi[x] = comp.size();
      comp.push_back({x});
      compCities.push_back(1);
    }
    unordered_map<int64_t, int64_t> mirror;
    for (int64_t i = 0; i < N; i++) mirror[(compi[2*i]<<32)+compi[2*i+1]] = 1;

    int64_t ret = 0;
    for (int64_t i = 0; i < M; i++) {
      cin >> A >> B;
      int64_t ac = compi[2*A-2], bc = compi[2*B-1];
      if (ac != bc) {
        if (comp[ac].size() < comp[bc].size()) swap(ac, bc);
        ret -= compCities[ac] * (compCities[ac]-1) / 2;
        ret -= compCities[bc] * (compCities[bc]-1) / 2;
        for (auto x : comp[bc]) {
          int64_t cc = compi[x^1];
          if (cc != bc) ret += --mirror[(min(bc, cc)<<32)+max(bc, cc)];
          compi[x] = ac;
          comp[ac].push_back(x);
          if (cc != ac) {
            compCities[ac]++;
            ret -= mirror[(min(ac, cc)<<32)+max(ac, cc)]++;
          }
        }
        ret += compCities[ac] * (compCities[ac]-1) / 2;
      }
      cout << ret << endl;
    }
  }
}
