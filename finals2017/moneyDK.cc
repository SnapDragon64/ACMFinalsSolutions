#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<pair<int, int>> normalize(vector<pair<int, int>> v) {
  vector<pair<int, int>> ret;
  sort(v.begin(), v.end());
  for (int i = 0; i < v.size(); i++) {
    while (ret.size() && ret.back().first <= v[i].first &&
                         ret.back().second <= v[i].second) {
      ret.pop_back();
    }
    ret.push_back(v[i]);
  }
  return ret;
}

vector<pair<int, int>> rev(vector<pair<int, int>> v) {
  reverse(v.begin(), v.end());
  for (int i = 0; i < v.size(); i++) v[i].first = -v[i].first;
  for (int i = 0; i < v.size(); i++) v[i].second = -v[i].second;
  return v;
}

long long doit(const vector<pair<int, int>>& a, int ai1, int ai2,
               const vector<pair<int, int>>& b, int bi1, int bi2,
               bool swapped) {
  if (ai2-ai1 <= 50 || bi2-bi1 <= 50) {
    long long ret = 0;
    for (int i = ai1; i < ai2; i++)
    for (int j = bi1; j < bi2; j++) {
      if (swapped) {
        ret = max(ret, (long long)min(0, b[j].first-a[i].first) *
                                  (b[j].second-a[i].second));
      } else {
        ret = max(ret, (long long)max(0, b[j].first-a[i].first) *
                                  (b[j].second-a[i].second));
      }
    }
    return ret;
  }

  vector<pair<int, int>> b1, b2;
  int i = (ai2+ai1)/2;
  for (int j = bi1; j+1 < bi2; j += 2) {
    long long v1 = (long long)(b[j].first-a[i].first) *
                              (b[j].second-a[i].second);
    long long v2 = (long long)(b[j+1].first-a[i].first) *
                              (b[j+1].second-a[i].second);
    if (v1 < v2) {
      b1.push_back(b[j]); b1.push_back(b[j+1]); b2.push_back(b[j+1]);
    } else {
      b1.push_back(b[j]); b2.push_back(b[j]); b2.push_back(b[j+1]);
    }
  }
  if ((bi2-bi1)%2) { b1.push_back(b[bi2-1]); b2.push_back(b[bi2-1]); }

  return max(doit(b1, 0, b1.size(), a, ai1, i, !swapped),
             doit(b2, 0, b2.size(), a, i, ai2, !swapped));
}

int main() {
  int M, N;
  while (cin >> M >> N) {
    vector<pair<int, int>> A(M), B(N);
    for (int i = 0; i < M; i++) cin >> A[i].first >> A[i].second;
    for (int i = 0; i < N; i++) cin >> B[i].first >> B[i].second;

    A = rev(normalize(rev(A)));
    B = normalize(B);

    cout << doit(A, 0, A.size(), B, 0, B.size(), false) << endl;
  }
}
