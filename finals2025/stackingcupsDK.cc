#include <iostream>
#include <vector>
using namespace std;

int main() {
  int64_t N, H;
  while (cin >> N >> H) {
    vector<int> used(N+1);
    if (N >= 4 && H == 2*N+1) {
      cout << 2*N-1 << ' ' << 3 << ' ' << 2*N-3;
      used[2] = used[N] = used[N-1] = true;
    } else if (H < 2*N-1 || H == 2*N+1 || H == N*N-2 || H > N*N) {
      cout << "impossible" << endl;
      continue;
    } else {
      for (int i = N; i >= 1; i--) if (H >= 2*i-1 && H != 2*i+1) {
        H -= 2*i-1;
        used[i] = true;
      }
      bool first = true;
      for (int i = 1; i <= N; i++) if (used[i]) {
        if (!first) cout << ' ';
        first = false;
        cout << 2*i-1;
      }
    }
    for (int i = N; i >= 1; i--) if (!used[i]) cout << ' ' << 2*i-1;
    cout << endl;
  }
}
