#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define HASHX 1049124381385522193LL
#define HASHMOD 1000000009

int main() {
  int N, S;
  while (cin >> N >> S) {
    vector<pair<pair<vector<int>, int>, string>> v;
    for (int j = 0; j < S; j++) {
      string s;
      cin >> s;
      long long fhash = 0, curv = 1, bhash = 0;
      long long fhash2 = 0, curv2 = 1, bhash2 = 0;
      vector<int> overlaps;
      for (int i = 0; i < s.size(); i++) {
        if (fhash == bhash && fhash2 == bhash2 && (!i || 2*s.size()-i <= N)) {
          overlaps.push_back(s.size()-i);
        }
        fhash += curv * (long long)s[i];
        fhash2 = (fhash2 + curv2 * (long long)s[i]) % HASHMOD;
        curv *= HASHX;
        curv2 = (curv2 * 42) % HASHMOD;
        bhash = bhash * HASHX + (long long)s[s.size()-1-i];
        bhash2 = (bhash2 * 42 + (long long)s[s.size()-1-i]) % HASHMOD;
      }
      reverse(overlaps.begin(), overlaps.end());
      v.push_back(make_pair(make_pair(overlaps, -j), s));
    }

    sort(v.begin(), v.end());
    reverse(v.begin(), v.end());
    for (auto it : v) cout << it.second << endl;
  }
}
