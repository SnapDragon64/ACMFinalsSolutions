#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N;
  while (cin >> N) {
    vector<int> A(N);
    for (int i = 0; i < N; i++) cin >> A[i];

    int tot = 0;
    for (int i = 0; i < N; i++) tot += A[i];

    int ret;
    for (ret = 100; ret; ret--) {
      vector<int> v = A;
      if (tot%2) goto fail;
      if (ret == 1 && tot != 2) goto fail;
      for (int i = ret, p1 = 0, p2 = v.size()-1; i; i--) {
        if (v[p1] < i) goto fail;
        v[p1] -= i;
        while (p1 < p2 && v[p1] == 0) p1++;
        if (v[p2] < i) goto fail;
        v[p2] -= i;
        while (p1 < p2 && v[p2] == 0) p2--;
      }

      break;
fail: ;
    }

    if (ret) {
      cout << ret << endl;
    } else {
      cout << "no quotation" << endl;
    }
  }
}
