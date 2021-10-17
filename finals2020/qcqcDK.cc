#include <algorithm>
#include <bitset>
#include <iostream>
#include <vector>
using namespace std;

// NOTE: This heuristic solution was written without regard for time limits (which were later set to be
// fairly aggressive).  It may need optimization to pass on Kattis.

int N;

struct Machine {
  bitset<100> tt, tf, ft, ff;
  bool operator==(const Machine& m) const { return tt == m.tt && tf == m.tf && ft == m.ft && ff == m.ff; }
  bool canBeTrue() const {
    return 2*tf.count() < N && (tt & tf).count() == 0;
  }
  bool canBeFalse() const {
    return 2*ff.count() < N && (ft & ff).count() == 0;
  }
  int score() const {
    return (canBeTrue()  ? tt.count()+tf.count() : 1000) +
           (canBeFalse() ? ft.count()+ff.count() : 1000);
  }
  friend ostream& operator<<(ostream& out, const Machine& m) {
    for (int i = 0; i < N; i++) out << m.tt[i]; out << ' ';
    for (int i = 0; i < N; i++) out << m.tf[i]; out << ' ';
    for (int i = 0; i < N; i++) out << m.ft[i]; out << ' ';
    for (int i = 0; i < N; i++) out << m.ff[i]; out << ' ';
    return out;
  }
};

void testTrue(Machine& m1, Machine& m2) {
  m1.tt |= m2.tt; m1.tf |= m2.tf;
  m2.ft |= m1.ft; m2.ff |= m1.ff;
}
void testFalse(Machine& m1, Machine& m2) {
  m1.tt |= m2.ft; m1.tf |= m2.ff;
  m2.tt |= m1.ft; m2.tf |= m1.ff;
}

void runTest(vector<Machine>& m, const vector<int>& v) {
  cout << "test";
  for (int i = 0; i < N; i++) cout << ' ' << v[i]+1;
  cout << endl;
  string s;
  cin >> s;
  for (;;) {
    vector<Machine> om = m;
    for (int i = 0; i < N; i++) {
      ((s[i] == '1') ? testTrue : testFalse)(m[i], m[v[i]]);
      if (!m[i].canBeTrue())  m[i].tf.set();
      if (!m[i].canBeFalse()) m[i].ff.set();
    }
    if (om == m) break;
  }
//for (int i = 0; i < N; i++) cout << m[i] << endl;
}

bool done(vector<Machine>& m) {
  for (int i = 0; i < m.size(); i++) if (m[i].canBeTrue() && m[i].canBeFalse()) return false;
  return true;
}

int main() {
  int B;
  for (cin >> B; B--;) {
    cin >> N;
    vector<Machine> m(N);
    for (int i = 0; i < N; i++) { m[i].tt.set(i); m[i].ff.set(i); }

    while (!done(m)) {
      vector<pair<int, pair<int, int>>> v;
      for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++) {
        int score1, score2;
        Machine m1 = m[i], m2 = m[j];
        testTrue(m1, m2);
        score1 = 1000*(m1.score()-m[i].score() + m2.score()-m[j].score()) + (i-j+N-1)%N;
        m1 = m[i], m2 = m[j];
        testFalse(m1, m2);
        score2 = 1000*(m1.score()-m[i].score() + m2.score()-m[j].score()) + (i-j+N-1)%N;
        v.push_back({-min(score1, score2), {i, j}});
      }
      sort(v.begin(), v.end());

      vector<int> test(N, -1), seen(N);
      for (auto const& it : v) if (test[it.second.first] == -1 && !seen[it.second.second]) {
        test[it.second.first] = it.second.second;
        seen[it.second.second] = true;
      }
      runTest(m, test);
    }

    cout << "answer ";
    for (int i = 0; i < N; i++) cout << m[i].canBeTrue();
    cout << endl;
  }
}
