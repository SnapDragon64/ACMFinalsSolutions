#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

// Pair of sets that implement a heap in two (shiftable) parts, with
// the ability to add a constant to either part in constant time.
struct DoubleHeap {
  long long set1Base, set2Base;
  multiset<long long> set1, set2;

  // Make sure set 1 contains exactly "part" elements, padding with 0 if needed.
  void shiftPartition(int part) {
    while (set1.size() < part) {
      if (set2.size()) {
        set1.insert(*set2.rbegin() + set2Base - set1Base);
        set2.erase(--set2.end());
      } else {
        set1.insert(-set1Base);
      }
    }
    while (set1.size() > part) {
      set2.insert(*set1.begin() + set1Base - set2Base);
      set1.erase(set1.begin());
    }
  }
  int size() {
    return set1.size() + set2.size();
  }
  void push(long long v) {
    if (!set2.size() || v - set2Base > *set2.rbegin()) {
      set1.insert(v - set1Base);
    } else {
      set2.insert(v - set2Base);
    }
  }
  void pop() {
    if (set1.size()) {
      set1.erase(--set1.end());
    } else {
      set2.erase(--set2.end());
    }
  }
  long long top() {
    if (set1.size()) {
      return *set1.rbegin() + set1Base;
    }
    return *set2.rbegin() + set2Base;
  }
  void pruneNeg() {
    while (set2.size() && *set2.begin() + set2Base < 0) {
      set2.erase(set2.begin());
    }
    while (set1.size() && *set1.begin() + set1Base < 0) {
      set1.erase(set1.begin());
    }
  }
};

vector<vector<int>> e, ec;
vector<int> X, Y;

struct State {
  int minInc;
  long long baseCost;
  DoubleHeap* savings;

  State() : minInc(0), baseCost(0), savings(new DoubleHeap()) {}
  ~State() { delete savings; }
};

State doit(int nd, int prev) {
  State st;
  for (int i = 0; i < e[nd].size(); i++) if (e[nd][i] != prev) {
    auto st2 = doit(e[nd][i], nd);
    st.minInc += st2.minInc;
    st.baseCost += st2.baseCost + (long long)ec[nd][i] * abs(st2.minInc);
    st2.savings->set1Base += ec[nd][i];
    st2.savings->set2Base -= ec[nd][i];
    st2.savings->pruneNeg();
    if (st2.savings->size() > st.savings->size()) swap(st.savings, st2.savings);
    while (st2.savings->size()) {
      long long v = st2.savings->top();
      if (v <= 0) break;
      st2.savings->pop();
      st.savings->push(v);
    }
  }
  st.minInc += Y[nd] - X[nd];
  st.savings->shiftPartition(max(0, -st.minInc));
  //cout << nd << ' ' << st.minInc << ' ' << st.baseCost << ' ';
  //DoubleHeap q = *st.savings;
  //while (q.size()) { if (q.top()) cout << ' ' << q.top(); q.pop(); }
  //cout << endl;
  return st;
}

int main() {
  int N;
  while (cin >> N) {
    e = ec = vector<vector<int>>(N+1);
    for (int i = 0; i < N-1; i++) {
      int A, B, C;
      cin >> A >> B >> C;
      e[A].push_back(B);
      e[B].push_back(A);
      ec[A].push_back(C);
      ec[B].push_back(C);
    }
    X = Y = vector<int>(N+1);
    for (int i = 1; i <= N; i++) cin >> X[i] >> Y[i];

    auto st = doit(1, -1);
    long long ret = st.baseCost;
    for (int i = 0; i < -st.minInc; i++) {
      ret -= st.savings->top();
      st.savings->pop();
    }
    cout << ret << endl;
  }
}
