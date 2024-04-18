#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<int64_t> B(N+1), E(N+1);
    for (int i = 1; i <= N; i++) cin >> B[i] >> E[i];
    vector<int64_t> S, T;
    for (int i = N, j = i; i > 0;) {
      if (j == 0) goto fail;
      int64_t t = B[j] - (E[i]-B[j]+1)/2;
      if (E[j-1] > t) { j--; continue; }
      if (E[j-1] >= t-1) {
        S.push_back(E[j-1]);
        T.push_back(B[j] - (E[j-1] == t-1 && E[i]-B[j]==1));
      } else {
        S.push_back(t);
        T.push_back(B[j]);
        S.push_back(E[j-1]);
        T.push_back((E[j-1]+t)/2);
      }
      i = --j;
    }
    cout << S.size() << endl;
    for (int i = S.size()-1; i >= 0; i--) cout << S[i] << ' ' << T[i] << endl;
    continue;
fail:
    cout << "impossible" << endl;
  }
}
