#include <iostream>
using namespace std;

main() {
  int T;
  for (cin >> T; T--;) {
    int L, V1, V2, Tol, S;
    cin >> L >> V1 >> V2 >> Tol >> S;

    int leaves = (V2-V1+Tol-1)/Tol, minv = V2-leaves*Tol, nodes = 1, depth = 0;
    while (nodes > 0 && leaves > nodes) {
      depth++;
      int maxv = L / (S*depth);
      int leaves2 = min(leaves, (maxv-minv+Tol)/Tol);
      nodes -= leaves - leaves2;
      nodes *= 2;
      leaves = leaves2;
    }

    if (nodes <= 0) {
      cout << "impossible" << endl;
    } else {
      cout << depth << endl;
    }
  }
}
