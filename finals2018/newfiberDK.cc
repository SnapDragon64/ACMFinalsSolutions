#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int N, M;
  while (cin >> N >> M) {
    vector<pair<int, int>> v(N);
    for (int i = 0; i < N; i++) v[i].second = i;
    for (int i = 0; i < M; i++) {
      int x, y;
      cin >> x >> y;
      v[x].first++; v[y].first++;
    }
    sort(v.begin(), v.end());

    int ret = 0, tot = M*2;
    for (int i = v.size()-1; tot > 2*N-2; i--) {
      int amt = min(tot-(2*N-2), v[i].first-1);
      tot -= amt;
      v[i].first -= amt;
      ret++;
    }
    sort(v.begin(), v.end());

    cout << ret << endl;
    cout << N << ' ' << N-1 << endl;
    if (N == 2) { cout << "0 1" << endl; continue; }
    vector<int> st;
    int i;
    for (i = 0; v[i].first == 1; i++) {
      st.push_back(v[i].second);
    }
    for (; i < v.size(); i++) {
      for (int j = 0; j < v[i].first-(i+1 < v.size()); j++) {
        cout << v[i].second << ' ' << st.back() << endl;
        st.pop_back();
      }
      st.push_back(v[i].second);
    }
  }
}
