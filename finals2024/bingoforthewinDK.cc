#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
  int N, K, X;
  while (cin >> N >> K) {
    map<int, pair<int, int>> num;
    for (int i = 0; i < N; i++)
    for (int j = 0; j < K; j++) {
      cin >> X;
      num[X].first = i;
      num[X].second++;
    }
    vector<int> fails(N);
    for (auto [x, p] : num) fails[p.first] += p.second;
    for (auto f : fails) cout << fixed << setprecision(10) << double(f)/N/K << endl;
  }
}
