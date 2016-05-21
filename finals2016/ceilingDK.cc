#include <iostream>
#include <vector>
using namespace std;

bool doit(const vector<int>& a, const vector<int>& b) {
  if (a.size() != b.size()) return false;
  if (a.size() <= 1) return true;
  vector<int> al, ar, bl, br;
  for (int i = 1; i < a.size(); i++) {
    (a[i] < a[0] ? al : ar).push_back(a[i]);
    (b[i] < b[0] ? bl : br).push_back(b[i]);
  }
  return doit(al, bl) && doit(ar, br);
}

int main() {
  int N, K;
  while (cin >> N >> K) {
    vector<vector<int>> tree(N, vector<int>(K));
    int ret = 0;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < K; j++) cin >> tree[i][j];
      int j;
      for (j = 0; j < i; j++) if (doit(tree[i], tree[j])) break;
      if (j == i) ret++;
    }
    cout << ret << endl;
  }
}
