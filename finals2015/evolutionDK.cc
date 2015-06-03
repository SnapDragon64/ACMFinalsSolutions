// Note: this is the solution written for DNA (as the problem was
// called then) for ICPC 2013, slightly modified for the new format. /Per

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int N;
string dna[4001];
vector<int> ret1, ret2;
short pred1[4001][4001], pred2[4001][4001];

bool lencmp(const string& a, const string& b) {
  return a.size() > b.size();
}

char memo[4001][4001];
bool subseq(int ai, int bi) {
  char& ret = memo[ai][bi];
  if (ret != -1) return ret;
  const string& a = dna[ai];
  const string& b = dna[bi];
  if (a.size() >= b.size()) return ret = false;
  int i, j;
  for (i = 0, j = 0; i < a.size() && j < b.size(); j++) {
    if (a[i] == b[j]) i++;
  }
  return ret = (i == a.size());
}

void doit(vector<int>& r1, vector<int>& r2, int i1, int i2) {
  if (i1 == 0 && i2 == 0) return;
  int p1 = pred1[i1][i2], p2 = pred2[i1][i2];
  if (p2 == i1) {
    doit(r2, r1, p1, p2);
  } else {
    doit(r1, r2, p1, p2);
  }
  r2.push_back(i2);
}

main() {
  while (cin >> N) {
    for (int i = 0; i <= N; i++) cin >> dna[i];
    sort(dna+1, dna+N+1, lencmp);

    vector<pair<int, int> > q;
    q.push_back(make_pair(0, 0));
    memset(memo, -1, sizeof(memo));
    memset(pred1, -1, sizeof(pred1));
    memset(pred2, -1, sizeof(pred2));
    while (q.size() && q.back().second < N) {
      int i1 = q.back().first, i2 = q.back().second;
      q.pop_back();
      if (pred1[i2][i2+1] == -1 && subseq(i2+1, i1)) {
        pred1[i2][i2+1] = i1;
        pred2[i2][i2+1] = i2;
        q.push_back(make_pair(i2, i2+1));
      }
      if (pred1[i1][i2+1] == -1 && subseq(i2+1, i2)) {
        pred1[i1][i2+1] = i1;
        pred2[i1][i2+1] = i2;
        q.push_back(make_pair(i1, i2+1));
      }
    }

    if (!q.size()) {
      cout << "Impossible" << endl;
      continue;
    }

    ret1.clear(); ret2.clear();
    doit(ret1, ret2, q.back().first, q.back().second);
    cout << ret1.size() << ' ' << ret2.size() << endl;
	reverse(ret1.begin(), ret1.end());
	reverse(ret2.begin(), ret2.end());
    for (int i = 0; i < ret1.size(); i++) cout << dna[ret1[i]] << endl;
    for (int i = 0; i < ret2.size(); i++) cout << dna[ret2[i]] << endl;
  }
}
