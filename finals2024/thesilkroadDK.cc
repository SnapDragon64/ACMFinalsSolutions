#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

inline int BitCount(unsigned x) {return __builtin_popcount(x);}

int main() {
  int N;
  while (cin >> N) {
    vector<int64_t> T(N), X(N), C(N);
    for (int i = 0; i < N; i++) {
      cin >> T[i] >> X[i];
      if (T[i] == 2) cin >> C[i];
    }

    vector<int64_t> coords = X;
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    do { coords.push_back(1e9); } while (BitCount(coords.size()-1) != 1);
    vector<int64_t> cr(coords.size()), cc(coords.size());

    enum {L2, L1, Z0, R1, R2};
    typedef vector<vector<int64_t>> Mat;
    Mat baseMat(5, vector<int64_t>(5, -1e18));
    auto makeMat = [&](int xi) {
      Mat ret = baseMat;
      int64_t seg = coords[xi+1] - coords[xi];
      ret[L2][L2] = ret[Z0][L2] = cc[xi] - 2*seg;
      ret[L1][L1] = ret[Z0][L1] = cc[xi] - seg;
      ret[Z0][Z0] = cr[xi] ? cc[xi] : 0;
      ret[R1][Z0] = ret[R2][Z0] = cc[xi];
      ret[R1][R1] = cc[xi] - seg;
      ret[R2][R2] = cc[xi] - 2*seg;
      if (cr[xi]) {
        ret[L1][R2] = cc[xi] - 2*seg;
        ret[L2][R1] = ret[Z0][R1] = cc[xi] - seg;
        ret[L1][Z0] = cc[xi];
      }
      return ret;
    };
    auto mul = [&](const Mat& a, const Mat& b) {
      Mat ret = baseMat;
      for (int s = 0; s < 5; s++)
      for (int m = 0; m < 5; m++)
      for (int e = 0; e < 5; e++) {
        ret[s][e] = max(ret[s][e], a[s][m]+b[m][e]);
      }
      return ret;
    };

    vector<vector<Mat>> segTree(1);
    for (int xi = 0; xi+1 < coords.size(); xi++) segTree[0].push_back(makeMat(xi));
    while (segTree.back().size() != 1) {
      const vector<Mat>& cur = segTree.back();
      vector<Mat> next;
      for (int i = 0; i+1 < cur.size(); i += 2) next.push_back(mul(cur[i], cur[i+1]));
      segTree.push_back(next);
    }

    for (int i = 0; i < N; i++) {
      int xi = lower_bound(coords.begin(), coords.end(), X[i]) - coords.begin();
      if (T[i] == 1) cr[xi]++; else cc[xi] += C[i];
      segTree[0][xi] = makeMat(xi);
      for (int j = 0; j+1 < segTree.size(); j++, xi /= 2) {
        segTree[j+1][xi/2] = mul(segTree[j][xi&~1], segTree[j][xi|1]);
      }
      cout << segTree.back()[0][Z0][Z0] << endl;
    }
  }
}
