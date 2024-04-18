#include <iomanip>
#include <iostream>
using namespace std;

int main() {
  int N, M;
  while (cin >> N >> M) {
    int A, tot = 0;
    for (int i = 0; i < N*M; i++) {
      cin >> A;
      tot += A;
    }
    cout << fixed << setprecision(10) << (long double)(tot) / (N*M) << endl;
  }
}
